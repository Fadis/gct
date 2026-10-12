/*
 * 
 *
 * This is a simplified version of Serial GBN stippling.
 * Original source code is available at https://doi.org/10.2312/cgvc.20241226
 *
 * = Copyright of original version =
 *
 * Serial GBN stippling.
 * This is a working demo code provided as a supplementary code with the paper:
 *      Serial Gaussian-Blue-Noise Stippling
 * The code is provided as is, without warrantees.
 * Apologies for the many testing options, as this is a research version.
 * Please contact me at abdalla_gafar@hotmail.com if you have questions
 *  or discussions.
 *
 * 2023-08-27: First serial (CPU) implementation by Abdalla Ahmed
 * 2023-06-18: Last revision for submission by Abdalla Ahmed
 */

#include <gct/gaussian_blue_noise.hpp>

#include <random>
#include <vector>
#include <cmath>
#include <stdint.h>
#include <numeric>                                                              // For std::accumulate
#include <algorithm>                                                            // For std::max_element
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

#define SUPPORT 6

namespace gct {

namespace {

std::random_device rd;                                                          // Used to obtain a seed for the random number engine
std::mt19937 rnd(rd());                                                         // Initialize random number generator
float rand(float x) {                                                           // Return a random number in [0, x)
    const float res = 1. / (1ull << 32);
    return res * rnd() * x;
}

// =============================================================================
// Image handling
// =============================================================================

struct Image {                                                                  // Data structure for holding image information
    int w, h;                                                                   // Width and height
    std::vector<float> pixels;                                                  // Image data
    float& operator[](int i) { return pixels[i]; };                             // Cast to pixel array
};

inline float frac(float x) { return x - std::floor(x); }

struct Rect {                                                                   // Rectangle
    float xl, xr, yb, yt;                                                       // left, right, bottom, top
    float width() { return xr - xl; };
    float height() { return yt - yb; };
};

class C2048 {
private:
    int w, h;                                                                   // Dimensions of accumulated mass buffer
    std::vector<float> accumMass;                                               // Mass of interval between origin and a given integer point.
    inline float m(int x, int y) { return accumMass[y * w + x]; }               // Shorthand
    inline float mass(float x, float y);                                        // Mass of of interval between origin and a given point.
    inline float mass(float xl, float xr, float yb, float yt) {                 // mass of delimited rectangle
        return mass(xr, yt) - mass(xl, yt) - mass(xr, yb) + mass(xl, yb);
    }
    inline float mass(Rect r) { return mass(r.xl, r.xr, r.yb, r.yt); };
    float hSect(float xl, float xr, float yb, float yt, float ratio = 0.5);     // Find vertical median line
    float vSect(float xl, float xr, float yb, float yt, float ratio = 0.5);     // Find horizontal median line
    glm::vec2 centroid(float xl, float xr, float yb, float yt);                     // Return center of mass of given rectangle
    glm::vec2 centroid(Rect r) { return centroid(r.xl, r.xr, r.yb, r.yt); } ;
public:
    C2048(const Image &img);
    std::vector< glm::vec2 > slice(int N);
};

// -----------------------------------------------------------------------------
// Constructor: initilaize accumulated mass buffer
// -----------------------------------------------------------------------------

C2048::C2048(const Image &img) : w(img.w + 1), h(img.h + 1) {
    accumMass.resize(h * w);
    for (int x = 0; x < w; x++) accumMass[x] = 0;                               // Accum mass is essentially zero on the bottom edge
    for (int y = 1; y < h; y++) {
        accumMass[y * w] = 0;                                                   // Accum mass is zero on the left edge
        for (int x = 1; x < w; x++) {
            accumMass[y * w + x] = (
                        img.pixels[(y - 1) * img.w + (x - 1)]                          // Pixel at origin of pixel area
                + accumMass[(y - 1) *     w + (x    )]                          // Accumulated mass one-row below
                + accumMass[(y    ) *     w + (x - 1)]                          // Accumulated mass one-column to the left
                - accumMass[(y - 1) *     w + (x - 1)]                          // Accumulated mass at origin, subtracted because it's added twice
            );
        }
    }
}

// -----------------------------------------------------------------------------
// Compute mass at a floating point
// -----------------------------------------------------------------------------

inline float C2048::mass(float x, float y) {                                    // mass of area to the left and below this point
    return (
        m(x, y)                                                                 // Mass of rectangle up to origin of containing pixel of the point
        + (m(x, ceil(y)) - m(x, y)) * frac(y)                                   // Mass of horizontal stripe above
        + (m(ceil(x), y) - m(x, y)) * frac(x)                                   // Mass of vertical stripe to the right
        + (m(ceil(x), ceil(y)) - m(x, ceil(y)) - m(ceil(x), y) + m(x, y))       // Mass of the containing pixel
            * frac(y) * frac(x)                                                 // Area of pixel below and to the left of the point
    );
}

// -----------------------------------------------------------------------------
// Section rectangle horizontally by a given ratio and return position
// of section line:
// -----------------------------------------------------------------------------

float C2048::hSect(float xl, float xr, float yb, float yt, float ratio) {
    float trgt = mass(xl, xr, yb, yt) * ratio;                                  // Target mass
    float x, mLeft, lowerBound(xl), upperBound(xr);
    while (upperBound - lowerBound > 1) {                                       // Binary search until at most one pixel boundary between ends
        x = 0.5 * (upperBound + lowerBound);                                    // Cut at midpoint
        mLeft = mass(xl, x, yb, yt);                                            // Mass of left half
        if (mLeft < trgt) lowerBound = x;                                       // If too small, section is in [x, upperBound]
        else           upperBound = x;                                          // Otherwise it is in [lowerBound, x]
    }
    lowerBound = floor(                                                         // We seek containing pixel origin
        mass(xl, floor(upperBound), yb, yt) < trgt ? upperBound : lowerBound    // Either the one containing upperBound or lowerBound
    );
    mLeft = mass(xl, lowerBound, yb, yt);                                       // Compute mass up to that edge, which is <= target mass
    x = lowerBound + (trgt - mLeft) / mass(lowerBound, lowerBound+1, yb, yt);   // Add fraction needed to reach target
    return x;
}

// -----------------------------------------------------------------------------
// Section given area vertically by given ratio and return position
// of section line:
// -----------------------------------------------------------------------------

float C2048::vSect (float xl, float xr, float yb, float yt, float ratio) {      // Analogous to hSect
    float trgt = mass(xl, xr, yb, yt) * ratio;
    float y, mBelow, lowerBound(yb), upperBound(yt);
    while (upperBound - lowerBound > 1) {
        y = 0.5 * (upperBound + lowerBound);
        mBelow = mass(xl, xr, yb, y);
        if (mBelow < trgt) lowerBound = y;
        else            upperBound = y;
    }
    lowerBound = floor(
        mass(xl, xr, yb, floor(upperBound)) < trgt ? upperBound : lowerBound
    );
    mBelow = mass(xl, xr, yb, lowerBound);
    y = lowerBound + (trgt - mBelow) / mass(xl, xr, lowerBound, lowerBound+1);
    return y;
}

// -----------------------------------------------------------------------------
// Compute centroid of a rectangle
// -----------------------------------------------------------------------------

glm::vec2 C2048::centroid(float xl, float xr, float yb, float yt) {
    float sumWeight, sum;
    glm::vec2 c;
    sumWeight = mass(xl, xr, yb, yt);
    if (int(xl) == int(xr)) {                                                   // All within one pixel?
        c.x = 0.5 * (xl + xr);                                                  // Then centroid is just at midpoint
    }
    else {
        sum = (
            mass(xl, ceil(xl), yb, yt) * (0.5 * (xl + ceil(xl))) +              // Stripe on the left
            mass(floor(xr), xr, yb, yt) * (0.5 * (xr + floor(xr)))              // Stripe on the right
        );
        for (int x = ceil(xl); x < floor(xr); x++) {                            // Iterate through pixels in between
            sum += mass(x, x + 1, yb, yt) * (x + 0.5);                          // Contribution of pixel column
        }
        c.x = sum / sumWeight;
    }
    if (int(yb) == int(yt)) {                                                   // All within one pixel?
        c.y = 0.5 * (yb + yt);                                                  // Then centroid is just at midpoint
    }
    else {
        sum = (
            mass(xl, xr, yb, ceil(yb)) * (0.5 * (yb + ceil(yb))) +              // Stripe at the bottom
            mass(xl, xr, floor(yt), yt) * (0.5 * (yt + floor(yt)))              // Stripe on top
        );
        for (int y = ceil(yb); y < floor(yt); y++) {                            // Iterate through pixels in between
            sum += mass(xl, xr, y, y + 1) * (y + 0.5);                          // Contribution of pixel row
        }
        c.y = sum / sumWeight;
    }
    return c;
}

// -----------------------------------------------------------------------------
// Slice recursively into equal-mass rectangles and place a samples
// in the centroid of each rectangle
// -----------------------------------------------------------------------------

std::vector< glm::vec2 > C2048::slice(int N) {
    std::vector< glm::vec2 > p;                                                                   // To store list of points
    if (N <= 0) return p;                                                       // Safety measure
    struct Record {                                                             // Used for stack-based recursion
        Rect rect;                                                              // Region
        int n;                                                                  // Number of points
    };
    std::vector<Record> stack;                                                  // LIFO, hence depth first
    Rect rect = {0.f, w - 1.f, 0.f, h - 1.f};                                           // Initial region is whole image
    stack.push_back({rect, N});                                                 // And whole number of points
    while (!stack.empty()) {
        Rect rect = stack.back().rect;                                          // Retrieve topmost region
        int n = stack.back().n;                                                 // And number of points
        stack.pop_back();                                                       // Remove from stack
        if (n == 1) {                                                           // Only one point?
            p.push_back(centroid(rect));                                        // Place sample at centroid, and we are done
        }
        else {                                                                  // More than one point
            int n1(n/2), n2(n - n1);                                            // Split roughly at halves
            float ratio = float(n1) / n;                                        // Splitting ratio, which is between 1/3 and 1/2
            if (rect.width() > rect.height()) {                                 // If landscape, slice horizontally
                float x = hSect(rect.xl, rect.xr, rect.yb, rect.yt, ratio);     // Section point
                stack.push_back({{rect.xl, x, rect.yb, rect.yt}, n1});          // Left half
                stack.push_back({{x, rect.xr, rect.yb, rect.yt}, n2});          // Right half
            }
            else {                                                              // Otherwise slice vertically
                float y = vSect(rect.xl, rect.xr, rect.yb, rect.yt, ratio);     // Section point
                stack.push_back({{rect.xl, rect.xr, rect.yb, y}, n1});          // Bottom half
                stack.push_back({{rect.xl, rect.xr, y, rect.yt}, n2});          // Top half
            }
        }
    }
    return p;
}


// =============================================================================
// Global Parameters
// =============================================================================

//float terminateThreshold = 0;
float stepCap = 0.5;                                                            // Relative to nearest neighbor distance
int scalingMode = 1;

// =============================================================================
// Utils
// =============================================================================

// -----------------------------------------------------------------------------
// Populate a randomly ordered list
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Return square distance between two points
// -----------------------------------------------------------------------------

inline float distSq(const glm::vec2 &p1, const glm::vec2 &p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return dx * dx + dy * dy;
}

// -----------------------------------------------------------------------------
// Return max travelled distance between two point sets
// -----------------------------------------------------------------------------

// =============================================================================
// Adaptive Class
// =============================================================================

class GBNAdaptive {
private:
    int N;                                                                      // Number of points
    std::vector<glm::vec2> p, pRef;                                                 // List of points
    float timeStep;                                                             // Scaling factor of gradient steps
    int w, h;                                                                   // Width and height of domain
    int n;                                                                      // Size, max(w, h), needed to maintain aspect ratio
    int initMode;                                                               // Initization mode
    float sigma;                                                                // Sigma of energy kernel
    float sigmaPixel;                                                           // Filtering sigma of pixels; mainly for testing; e.g. 0 means Dirac deltas
    std::vector<float> bmp;                                                     // Pixels
    std::vector<float> v;                                                       // kernel shaping factors (area)
    std::vector<int> nearestNeighbor;                                           // Index of nearest neighbor
    std::vector<int> neighbors;                                                 // List of considered neighbors for each site
    std::vector<int> indx0;                                                     // List of first index of each site in neighbors list
    float unitArea, unitAreaInv;
    void init();                                                                // Initialize parameters and buffers
    inline void updateKernel(int i);
    void initKernels();                                                         // Initialize kernel shaping factors
    int updateNeighborsList();
    // -------------------------------------------------------------------------
    // Recording parameters
    // -------------------------------------------------------------------------
    float outBlackRatio;                                                        // Ratio of black in produced renderings; 1 is full black
    float outputResolution;                                                     // For deciding if there is a noticeable difference
    int frameIndex;                                                             // Index of output frame
    int frameCount;                                                             // If set, frames would cycle through this loop size, rewriting older ones
    int accumIterations;                                                        // Statistics
    int W, H;                                                                   // Width and height of recorded frames
    float margin;                                                               // Margin in plots, given as a ratio to domain size
    float unit;                                                                 // Unit of recorded frames
public:
    GBNAdaptive(                                                                // Construct from scratch
        int N,                                                                  // Number of points
        Image img,                                                              // Density map
        int initMode = 0                                                        // 0: full-random, 1: weighted random, 2: 2048
    );
    void setSigma(float value);                                                 // Set sigma to a given value
    void setSigmaPixel(float value) { sigmaPixel = value; };
    void setTimeStep(float value) {timeStep = value;};
    void setFrameSize(int W, int H);
    void optimize(int iterations);
    std::vector< glm::vec2 > get_result() const {
      std::vector< glm::vec2 > temp;
      temp.reserve( N );
      std::transform(
        p.begin(), p.end(), std::back_inserter( temp ),
        [&]( const auto &v ) { return glm::vec2( v.x * unit, v.y * unit ); }
      );
      return temp;
    }
    ~GBNAdaptive();
};


// -----------------------------------------------------------------------------
// Construct from scratch
// -----------------------------------------------------------------------------

GBNAdaptive::GBNAdaptive(
    int N, Image img, int initMode
) : N(N), bmp(img.pixels), w(img.w), h(img.h), initMode(initMode) {
    init();                                                                     // Common initialization tasks
    switch (initMode) {
        case 0: {
            for (int i = 0; i < N; i++) { p[i] = {rand(w), rand(h)}; }
            break;
        }
        case 1: default: {
            float maxPixel = *std::max_element(bmp.begin(), bmp.end());         // Highest pixel density; high is darker
            for (int i = 0; i < N; /*conditionally increment inside loop*/) {   // Insert points proportional to density using rejection sampling
                float x = rand(w);                                              // Random x in [0, w)
                float y = rand(h);                                              // Random y in [0, h)
                float threshold = rand(maxPixel);                               // Random threshold
                if (bmp[int(y) * w + int(x)] > threshold) {                     // Is underlying pixel above threshold?
                    p[i++] = {x, y};                                            // If yes, insert a point at this location
                }
            }
            break;
        }
        case 2: {
            C2048 slicer(img);
            p = slicer.slice(N);
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Initialize parameters and buffers
// -----------------------------------------------------------------------------

void GBNAdaptive::init() {
    unitArea = float(h * w) / N;
    unitAreaInv = 1 / unitArea;
    p.resize(N);
    v.resize(N);
    nearestNeighbor.resize(N);
    neighbors.resize(500 * N);                                                  // Assuming 20x20 grid around each site plus a safety margin
    indx0.resize(N + 1);                                                        // We need to find the end for the last site.
    // -------------------------------------------------------------------------
    // Recording parameters
    // -------------------------------------------------------------------------
    n = std::max(w, h);                                                         // Square domain size to preserve aspect ratio
    unit = 1. / n;
    setFrameSize(w, h);                                                         // Default saved frame size to input image size
    accumIterations = 0;                                                        // Reset recording parameters
    frameIndex = 0;
    margin = 0;
    // -------------------------------------------------------------------------
    float mass = std::accumulate(bmp.begin(), bmp.end(), float(0));             // Sum of all pixels
    outBlackRatio = mass / (w * h);                                             // Default output rendering black level to match input image
    for (int i = 0; i < w * h; i++) { bmp[i] *= (N / mass); }                   // Make mass of pixels = mass of points
    float maxDensity = *std::max_element(bmp.begin(), bmp.end()) * unitArea;
    setSigmaPixel(0.5);                                                         // 0: deltas; < 0 means use same sigma
    setSigma(1);
    //setTimeStep(0.5);                                                           // Our default, after long experimentation
    setTimeStep(0.5 / maxDensity);
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------

GBNAdaptive::~GBNAdaptive() {
}

// -----------------------------------------------------------------------------
// Set energy sigma
// -----------------------------------------------------------------------------

void GBNAdaptive::setSigma(float value) {
    if (value == 93) value = 0.930604859102099599;                              // sqrt(sqrt(3)/2), indicating triangular grid
    sigma = value                                                               // Input energy kernel sigma
            / sqrt(2)                                                           // This is the filtering kernel
            / sqrt(2)                                                           // Account for the pixels, 50% share
            * sqrt(unitArea)                                                    // Scale to point area
    ;
}

// -----------------------------------------------------------------------------
// Compute list of neighbors
// -----------------------------------------------------------------------------

int GBNAdaptive::updateNeighborsList() {
    const float supportSqRef = SUPPORT * SUPPORT * sigma * sigma;
    float ssRef = sigma * sigma;
    int k = 0;
    int neighborCountMax(0);
    for (int i = 0; i < N; i++) {
        indx0[i] = k;
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            float supportSq = supportSqRef * (v[i] + v[j]);
            float rr = distSq(p[i], p[j]);
            if (rr < supportSq) {
                neighbors[k++] = j;                                             // Register j as a neighbor of i.
            }
        }
        neighborCountMax = std::max(neighborCountMax, k - indx0[i]);
    }
    indx0[N] = k;                                                               // Cap
    return neighborCountMax;
}


// -------------------------------------------------------------------------
// Update kernel shaping factor of a specific point
// -------------------------------------------------------------------------

void GBNAdaptive::updateKernel(int i) {
    float ddMin(1e12);
    int minIndex(-1);
    for (int k = indx0[i]; k < indx0[i+1]; k++) {
        int j = neighbors[k];
        float dd = distSq(p[i], p[j]);
        if (ddMin > dd) {
            ddMin = dd;
            minIndex = j;
        }

    }
    v[i] = ddMin * unitAreaInv;
    nearestNeighbor[i] = minIndex;
}

// -------------------------------------------------------------------------
// Initialize kernel shaping factors
// -------------------------------------------------------------------------
void GBNAdaptive::initKernels() {
    for (int i = 0; i < N; i++) {
        float ddMin(1e12);
        int minIndex(-1);
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            float dd = distSq(p[i], p[j]);
            if (ddMin > dd) {
                ddMin = dd;
                minIndex = j;
            }
        }
        v[i] = ddMin * unitAreaInv;
        nearestNeighbor[i] = minIndex;
    }
}

// -----------------------------------------------------------------------------
// Optimization Routine
// -----------------------------------------------------------------------------


void GBNAdaptive::optimize(int iterations) {
    // -------------------------------------------------------------------------
    // Set optimization parameters
    // -------------------------------------------------------------------------
    float ss2InvNeg = -1 / (2 * sigma * sigma);
    initKernels();
    if (sigmaPixel < 0) {                                                       // Instruction to use the same sigma as the points, noting that ddMin of pixels is 1
        sigmaPixel = sigma * sqrt(unitAreaInv);
    }
    float vPixel = (sigmaPixel/sigma) * (sigmaPixel/sigma);
    int neighborCountMax = updateNeighborsList();
    const float NInv = float(1) / N;
    std::vector< glm::vec2 > pNeighborsRef = p;                                                   // Needed to update neighbors list
    pRef = p;                                                                   // Copy point set as a reference for recording
    float stepCapSq = stepCap * stepCap * unitArea;
    // -------------------------------------------------------------------------
    // Optimization loop
    // -------------------------------------------------------------------------
    std::vector<float> gx(w), ex(w), gy(h), ey(h);
    float gradLength, gradLengthMax, gradLengthAvg(n), gradLengthSum;
    std::vector< glm::vec2 > pNext;                                                               // For parallel mode
    float timeStep = this->timeStep;
    //if (2 * sigma > sqrt(unitArea)) {                                           // If supplied sigma > 1
        timeStep *= unitArea / (4 * sigma * sigma);
    //}
    for (int itrt = 0; itrt < iterations; itrt++) {
        float stepSqMax(0);                                                     // Square of largest displacement in this iteration
        int reIterated(0);
        int clampedCount(0);
        gradLengthSum = 0;
        gradLengthMax = 0;
        for (int i = 0; i < N; i++) {                                           // Iterate through all points
            if (distSq(p[i], pNeighborsRef[i]) > 0.25 * unitArea * v[i]) {      // If a point moves more than half distance to nearest neighbor
                neighborCountMax = updateNeighborsList();                       // Update all neighbors map
                pNeighborsRef = p;                                              // Update reference locations
                break;                                                          // No need to continue with the loop, then
            }
        }
        std::vector<int> order(N);
        for (int k = 0; k < N; k++) {                         // Iterate through all points
            int i = k;
            glm::vec2 grad = glm::vec2{0, 0};                                               // Gradient
            float t = timeStep;
            if (scalingMode) t *= pow(v[i], 0.5 * scalingMode);
            // -----------------------------------------------------------------
            // Attraction of image
            // -----------------------------------------------------------------
            float support = SUPPORT * sigma * std::sqrt(v[i] + vPixel);
            int Xmin = std::max(floor(p[i].x - support), float(    0));
            int Xmax = std::min( ceil(p[i].x + support), float(w - 1));
            int Ymin = std::max(floor(p[i].y - support), float(    0));
            int Ymax = std::min( ceil(p[i].y + support), float(h - 1));
            float a = 1 / (v[i] + vPixel);                                      // Kernel shaping factor
            ///*test*/ a = 1 / (v[i] + v[i] * unitAreaInv);
            //float a = 1 / ((1 + vPixel)*v[i]);                                  // Kernel shaping factor
            {                                                              // Sample gradient at pixel points
                for (int Y = Ymin, row = 0; Y <= Ymax; Y++, row++) {
                    float dy = p[i].y - (Y + float(0.5));
                    gy[row] = std::exp(a * ss2InvNeg * dy * dy);
                }
                for (int X = Xmin, col = 0; X <= Xmax; X++, col++) {
                    float dx = p[i].x - (X + float(0.5));
                    gx[col] = std::exp(a * ss2InvNeg * dx * dx);
                }
                for (int Y = Ymin, row = 0; Y <= Ymax; Y++, row++) {
                    float dy = p[i].y - (Y + float(0.5));
                    for (int X = Xmin, col = 0; X <= Xmax; X++, col++) {
                        float dx = p[i].x - (X + float(0.5));
                        float g = gx[col] * gy[row];
                        g *= bmp[Y * w + X];
                        grad.x -= dx * g;
                        grad.y -= dy * g;
                    }
                }
            }
            grad *= a * a;
            // -----------------------------------------------------------------
            // Repulsion of other points:
            // -----------------------------------------------------------------
            for (int k = indx0[i]; k < indx0[i+1]; k++) {
                int j = neighbors[k];
                float dx = p[i].x - p[j].x;
                float dy = p[i].y - p[j].y;
                float rr = dx * dx + dy * dy;
                float a = 1 / (v[i] + v[j]);
                float g = a * a * std::exp(a * ss2InvNeg * rr);
                grad.x += dx * g;
                grad.y += dy * g;
            }
            // -----------------------------------------------------------------
            // Update point:
            // -----------------------------------------------------------------
            gradLength = glm::length( grad );
            gradLengthSum += gradLength;
            gradLengthMax = std::max(gradLengthMax, gradLength);
            grad *= t;
            float displacementMaxSq = stepCapSq * v[i];                         // Cap relative to nearest neighbor distance
            float sq = glm::dot( grad, grad );
            if ( sq > displacementMaxSq ) {
                grad *= std::sqrt( displacementMaxSq / sq );               // Clamp to max allowed displacement
                clampedCount++;                                                 // Collect information if needed
            }
            float x = p[i].x + grad.x;
            float y = p[i].y + grad.y;
            if (x < 0 || x >= w || y < 0 || y >= h) continue;                   // Restrict to image boundary. In theory it should not be needed
            sq = glm::dot( grad, grad );
            stepSqMax = std::max( stepSqMax, sq );
            // -----------------------------------------------------------------
            // Update point location
            // -----------------------------------------------------------------
            p[i] = {x, y};
            // -----------------------------------------------------------------
            // Update shaping factor of self and affected neighbors
            // -----------------------------------------------------------------
            float ddMin(1e12);
            int minIndex(-1);
            for (int k = indx0[i]; k < indx0[i+1]; k++) {
                int j = neighbors[k];
                float dd = distSq(p[i], p[j]);
                if (ddMin > dd) {
                    ddMin = dd;
                    minIndex = j;
                }
                if (dd * unitAreaInv < v[j]) {
                    v[j] = dd * unitAreaInv;
                    nearestNeighbor[j] = i;
                }
                if (nearestNeighbor[j] == i && dd * unitAreaInv > v[j]) {       // Was nearest neighbor and moved away
                    updateKernel(j);                                            // Search for a new nearest neighbor
                }
            }
            v[i] = ddMin * unitAreaInv;
            nearestNeighbor[i] = minIndex;
        }
        gradLengthAvg = gradLengthSum * NInv;
        //if (gradLengthMax < terminateThreshold) break;
        //if (sqrt(stepSqMax) / outputResolution < terminateThreshold) break;
    }
}

void GBNAdaptive::setFrameSize(int W, int H) {
    this->W = W;
    this->H = H;
    outputResolution = (float(n) / std::max(W, H));                             // Pixel in output resolution
}

}

std::vector< glm::vec2 > gaussian_blue_noise(
  const std::vector< float > &dist,
  int width,
  int point_count,
  int iterations
) {
  Image img;
  img.w = width;
  img.h = dist.size() / width;
  img.pixels = dist;
  
  GBNAdaptive gbn = GBNAdaptive( point_count, img, 1 );
  gbn.optimize(iterations);
  return gbn.get_result();
}

std::vector< glm::vec2 > gaussian_blue_noise(
  int point_count,
  int iterations
) {
  std::vector< float > dist( 16 * 16, 1.0f );
  return gaussian_blue_noise( dist, 16, point_count, iterations );
}

}

