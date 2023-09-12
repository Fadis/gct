#ifndef GCT_COLOR_H
#define GCT_COLOR_H
/*
 * Copyright (C) 2021 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <array>
namespace gct::color::web {
constexpr std::array< float, 4u > black{ 0.000000f, 0.000000f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > dimgray{ 0.141263f, 0.141263f, 0.141263f, 1.0f };
constexpr std::array< float, 4u > dimgrey{ 0.141263f, 0.141263f, 0.141263f, 1.0f };
constexpr std::array< float, 4u > gray{ 0.215861f, 0.215861f, 0.215861f, 1.0f };
constexpr std::array< float, 4u > grey{ 0.215861f, 0.215861f, 0.215861f, 1.0f };
constexpr std::array< float, 4u > darkgray{ 0.396755f, 0.396755f, 0.396755f, 1.0f };
constexpr std::array< float, 4u > darkgrey{ 0.396755f, 0.396755f, 0.396755f, 1.0f };
constexpr std::array< float, 4u > silver{ 0.527115f, 0.527115f, 0.527115f, 1.0f };
constexpr std::array< float, 4u > lightgray{ 0.651406f, 0.651406f, 0.651406f, 1.0f };
constexpr std::array< float, 4u > lightgrey{ 0.651406f, 0.651406f, 0.651406f, 1.0f };
constexpr std::array< float, 4u > gainsboro{ 0.715694f, 0.715694f, 0.715694f, 1.0f };
constexpr std::array< float, 4u > lightslategray{ 0.184475f, 0.246201f, 0.318547f, 1.0f };
constexpr std::array< float, 4u > lightslategrey{ 0.184475f, 0.246201f, 0.318547f, 1.0f };
constexpr std::array< float, 4u > slategray{ 0.162029f, 0.215861f, 0.278894f, 1.0f };
constexpr std::array< float, 4u > slategrey{ 0.162029f, 0.215861f, 0.278894f, 1.0f };
constexpr std::array< float, 4u > midnightblue{ 0.009721f, 0.009721f, 0.162029f, 1.0f };
constexpr std::array< float, 4u > navy{ 0.000000f, 0.000000f, 0.215861f, 1.0f };
constexpr std::array< float, 4u > darkblue{ 0.000000f, 0.000000f, 0.258183f, 1.0f };
constexpr std::array< float, 4u > mediumblue{ 0.000000f, 0.000000f, 0.610496f, 1.0f };
constexpr std::array< float, 4u > blue{ 0.000000f, 0.000000f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > royalblue{ 0.052861f, 0.141263f, 0.752942f, 1.0f };
constexpr std::array< float, 4u > dodgerblue{ 0.012983f, 0.278894f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > steelblue{ 0.061246f, 0.223228f, 0.456411f, 1.0f };
constexpr std::array< float, 4u > cornflowerblue{ 0.127438f, 0.300544f, 0.846873f, 1.0f };
constexpr std::array< float, 4u > deepskyblue{ 0.000000f, 0.520996f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > lightskyblue{ 0.242281f, 0.617207f, 0.955973f, 1.0f };
constexpr std::array< float, 4u > skyblue{ 0.242281f, 0.617207f, 0.830770f, 1.0f };
constexpr std::array< float, 4u > powderblue{ 0.434154f, 0.745404f, 0.791298f, 1.0f };
constexpr std::array< float, 4u > lightblue{ 0.417885f, 0.686685f, 0.791298f, 1.0f };
constexpr std::array< float, 4u > lightsteelblue{ 0.434154f, 0.623960f, 0.730461f, 1.0f };
constexpr std::array< float, 4u > lavender{ 0.791298f, 0.791298f, 0.955973f, 1.0f };
constexpr std::array< float, 4u > aliceblue{ 0.871367f, 0.938686f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > ghostwhite{ 0.938686f, 0.938686f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > azure{ 0.871367f, 1.000000f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > lightcyan{ 0.745404f, 1.000000f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > aqua{ 0.000000f, 1.000000f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > cyan{ 0.000000f, 1.000000f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > paleturquoise{ 0.428690f, 0.854993f, 0.854993f, 1.0f };
constexpr std::array< float, 4u > mediumaquamarine{ 0.132868f, 0.610496f, 0.401978f, 1.0f };
constexpr std::array< float, 4u > aquamarine{ 0.212231f, 1.000000f, 0.658375f, 1.0f };
constexpr std::array< float, 4u > turquoise{ 0.051269f, 0.745404f, 0.630757f, 1.0f };
constexpr std::array< float, 4u > mediumturquoise{ 0.064803f, 0.637597f, 0.603827f, 1.0f };
constexpr std::array< float, 4u > darkturquoise{ 0.000000f, 0.617207f, 0.637597f, 1.0f };
constexpr std::array< float, 4u > lightseagreen{ 0.014444f, 0.445201f, 0.401978f, 1.0f };
constexpr std::array< float, 4u > cadetblue{ 0.114435f, 0.341914f, 0.351533f, 1.0f };
constexpr std::array< float, 4u > darkcyan{ 0.000000f, 0.258183f, 0.258183f, 1.0f };
constexpr std::array< float, 4u > teal{ 0.000000f, 0.215861f, 0.215861f, 1.0f };
constexpr std::array< float, 4u > darkslategray{ 0.018500f, 0.039546f, 0.045186f, 1.0f };
constexpr std::array< float, 4u > darkslategrey{ 0.018500f, 0.039546f, 0.045186f, 1.0f };
constexpr std::array< float, 4u > seagreen{ 0.027321f, 0.258183f, 0.095307f, 1.0f };
constexpr std::array< float, 4u > mediumseagreen{ 0.045186f, 0.450786f, 0.165132f, 1.0f };
constexpr std::array< float, 4u > olivedrab{ 0.147027f, 0.270498f, 0.016807f, 1.0f };
constexpr std::array< float, 4u > olive{ 0.215861f, 0.215861f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > darkolivegreen{ 0.090842f, 0.147027f, 0.028426f, 1.0f };
constexpr std::array< float, 4u > forestgreen{ 0.015996f, 0.258183f, 0.015996f, 1.0f };
constexpr std::array< float, 4u > green{ 0.000000f, 0.215861f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > darkgreen{ 0.000000f, 0.127438f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > limegreen{ 0.031896f, 0.610496f, 0.031896f, 1.0f };
constexpr std::array< float, 4u > darkseagreen{ 0.274677f, 0.502886f, 0.274677f, 1.0f };
constexpr std::array< float, 4u > yellowgreen{ 0.323143f, 0.610496f, 0.031896f, 1.0f };
constexpr std::array< float, 4u > springgreen{ 0.000000f, 1.000000f, 0.212231f, 1.0f };
constexpr std::array< float, 4u > mediumspringgreen{ 0.000000f, 0.955973f, 0.323143f, 1.0f };
constexpr std::array< float, 4u > lime{ 0.000000f, 1.000000f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > lawngreen{ 0.201556f, 0.973445f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > chartreuse{ 0.212231f, 1.000000f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > greenyellow{ 0.417885f, 1.000000f, 0.028426f, 1.0f };
constexpr std::array< float, 4u > lightgreen{ 0.278894f, 0.854993f, 0.278894f, 1.0f };
constexpr std::array< float, 4u > palegreen{ 0.313989f, 0.964686f, 0.313989f, 1.0f };
constexpr std::array< float, 4u > honeydew{ 0.871367f, 1.000000f, 0.871367f, 1.0f };
constexpr std::array< float, 4u > mintcream{ 0.913099f, 1.000000f, 0.955973f, 1.0f };
constexpr std::array< float, 4u > lemonchiffon{ 1.000000f, 0.955973f, 0.610496f, 1.0f };
constexpr std::array< float, 4u > lightgoldenrodyellow{ 0.955973f, 0.955973f, 0.644480f, 1.0f };
constexpr std::array< float, 4u > lightyellow{ 1.000000f, 1.000000f, 0.745404f, 1.0f };
constexpr std::array< float, 4u > beige{ 0.913099f, 0.913099f, 0.715694f, 1.0f };
constexpr std::array< float, 4u > cornsilk{ 1.000000f, 0.938686f, 0.715694f, 1.0f };
constexpr std::array< float, 4u > antiquewhite{ 0.955973f, 0.830770f, 0.679542f, 1.0f };
constexpr std::array< float, 4u > papayawhip{ 1.000000f, 0.863157f, 0.665387f, 1.0f };
constexpr std::array< float, 4u > blanchedalmond{ 1.000000f, 0.830770f, 0.610496f, 1.0f };
constexpr std::array< float, 4u > bisque{ 1.000000f, 0.775822f, 0.552011f, 1.0f };
constexpr std::array< float, 4u > wheat{ 0.913099f, 0.730461f, 0.450786f, 1.0f };
constexpr std::array< float, 4u > moccasin{ 1.000000f, 0.775822f, 0.462077f, 1.0f };
constexpr std::array< float, 4u > peachpuff{ 1.000000f, 0.701102f, 0.485150f, 1.0f };
constexpr std::array< float, 4u > navajowhite{ 1.000000f, 0.730461f, 0.417885f, 1.0f };
constexpr std::array< float, 4u > palegoldenrod{ 0.854993f, 0.806952f, 0.401978f, 1.0f };
constexpr std::array< float, 4u > khaki{ 0.871367f, 0.791298f, 0.262251f, 1.0f };
constexpr std::array< float, 4u > yellow{ 1.000000f, 1.000000f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > gold{ 1.000000f, 0.679542f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > orange{ 1.000000f, 0.376262f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > sandybrown{ 0.904661f, 0.371238f, 0.116971f, 1.0f };
constexpr std::array< float, 4u > burlywood{ 0.730461f, 0.479320f, 0.242281f, 1.0f };
constexpr std::array< float, 4u > tan{ 0.644480f, 0.456411f, 0.262251f, 1.0f };
constexpr std::array< float, 4u > darkkhaki{ 0.508881f, 0.473531f, 0.147027f, 1.0f };
constexpr std::array< float, 4u > goldenrod{ 0.701102f, 0.376262f, 0.014444f, 1.0f };
constexpr std::array< float, 4u > darkgoldenrod{ 0.479320f, 0.238398f, 0.003347f, 1.0f };
constexpr std::array< float, 4u > peru{ 0.610496f, 0.234551f, 0.049707f, 1.0f };
constexpr std::array< float, 4u > sienna{ 0.351533f, 0.084376f, 0.026241f, 1.0f };
constexpr std::array< float, 4u > saddlebrown{ 0.258183f, 0.059511f, 0.006512f, 1.0f };
constexpr std::array< float, 4u > chocolate{ 0.644480f, 0.141263f, 0.012983f, 1.0f };
constexpr std::array< float, 4u > darkorange{ 1.000000f, 0.262251f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > coral{ 1.000000f, 0.212231f, 0.080220f, 1.0f };
constexpr std::array< float, 4u > lightsalmon{ 1.000000f, 0.351533f, 0.194618f, 1.0f };
constexpr std::array< float, 4u > darksalmon{ 0.814847f, 0.304987f, 0.194618f, 1.0f };
constexpr std::array< float, 4u > salmon{ 0.955973f, 0.215861f, 0.168269f, 1.0f };
constexpr std::array< float, 4u > lightcoral{ 0.871367f, 0.215861f, 0.215861f, 1.0f };
constexpr std::array< float, 4u > indianred{ 0.610496f, 0.107023f, 0.107023f, 1.0f };
constexpr std::array< float, 4u > tomato{ 1.000000f, 0.124772f, 0.063010f, 1.0f };
constexpr std::array< float, 4u > orangered{ 1.000000f, 0.059511f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > red{ 1.000000f, 0.000000f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > firebrick{ 0.445201f, 0.015996f, 0.015996f, 1.0f };
constexpr std::array< float, 4u > brown{ 0.376262f, 0.023153f, 0.023153f, 1.0f };
constexpr std::array< float, 4u > darkred{ 0.258183f, 0.000000f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > maroon{ 0.215861f, 0.000000f, 0.000000f, 1.0f };
constexpr std::array< float, 4u > rosybrown{ 0.502886f, 0.274677f, 0.274677f, 1.0f };
constexpr std::array< float, 4u > mistyrose{ 1.000000f, 0.775822f, 0.752942f, 1.0f };
constexpr std::array< float, 4u > pink{ 1.000000f, 0.527115f, 0.597202f, 1.0f };
constexpr std::array< float, 4u > lightpink{ 1.000000f, 0.467784f, 0.533276f, 1.0f };
constexpr std::array< float, 4u > palevioletred{ 0.708376f, 0.162029f, 0.291771f, 1.0f };
constexpr std::array< float, 4u > hotpink{ 1.000000f, 0.141263f, 0.456411f, 1.0f };
constexpr std::array< float, 4u > deeppink{ 1.000000f, 0.006995f, 0.291771f, 1.0f };
constexpr std::array< float, 4u > crimson{ 0.715694f, 0.006995f, 0.045186f, 1.0f };
constexpr std::array< float, 4u > mediumvioletred{ 0.571125f, 0.007499f, 0.234551f, 1.0f };
constexpr std::array< float, 4u > orchid{ 0.701102f, 0.162029f, 0.672443f, 1.0f };
constexpr std::array< float, 4u > violet{ 0.854993f, 0.223228f, 0.854993f, 1.0f };
constexpr std::array< float, 4u > fuchsia{ 1.000000f, 0.000000f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > magenta{ 1.000000f, 0.000000f, 1.000000f, 1.0f };
constexpr std::array< float, 4u > mediumorchid{ 0.491021f, 0.090842f, 0.651406f, 1.0f };
constexpr std::array< float, 4u > slateblue{ 0.144128f, 0.102242f, 0.610496f, 1.0f };
constexpr std::array< float, 4u > mediumslateblue{ 0.198069f, 0.138432f, 0.854993f, 1.0f };
constexpr std::array< float, 4u > darkslateblue{ 0.064803f, 0.046665f, 0.258183f, 1.0f };
constexpr std::array< float, 4u > indigo{ 0.070360f, 0.000000f, 0.223228f, 1.0f };
constexpr std::array< float, 4u > rebeccapurple{ 0.132868f, 0.033105f, 0.318547f, 1.0f };
constexpr std::array< float, 4u > darkmagenta{ 0.258183f, 0.000000f, 0.258183f, 1.0f };
constexpr std::array< float, 4u > purple{ 0.215861f, 0.000000f, 0.215861f, 1.0f };
constexpr std::array< float, 4u > darkorchid{ 0.318547f, 0.031896f, 0.603827f, 1.0f };
constexpr std::array< float, 4u > darkviolet{ 0.296138f, 0.000000f, 0.651406f, 1.0f };
constexpr std::array< float, 4u > blueviolet{ 0.254152f, 0.024158f, 0.760525f, 1.0f };
constexpr std::array< float, 4u > mediumpurple{ 0.291771f, 0.162029f, 0.708376f, 1.0f };
constexpr std::array< float, 4u > plum{ 0.723055f, 0.351533f, 0.723055f, 1.0f };
constexpr std::array< float, 4u > thistle{ 0.686685f, 0.520996f, 0.686685f, 1.0f };
constexpr std::array< float, 4u > lavenderblush{ 1.000000f, 0.871367f, 0.913099f, 1.0f };
constexpr std::array< float, 4u > oldlace{ 0.991102f, 0.871367f, 0.768151f, 1.0f };
constexpr std::array< float, 4u > linen{ 0.955973f, 0.871367f, 0.791298f, 1.0f };
constexpr std::array< float, 4u > seashell{ 1.000000f, 0.913099f, 0.854993f, 1.0f };
constexpr std::array< float, 4u > floralwhite{ 1.000000f, 0.955973f, 0.871367f, 1.0f };
constexpr std::array< float, 4u > ivory{ 1.000000f, 1.000000f, 0.871367f, 1.0f };
constexpr std::array< float, 4u > whitesmoke{ 0.913099f, 0.913099f, 0.913099f, 1.0f };
constexpr std::array< float, 4u > snow{ 1.000000f, 0.955973f, 0.955973f, 1.0f };
constexpr std::array< float, 4u > white{ 1.000000f, 1.000000f, 1.000000f, 1.0f };
}
namespace gct::color::special {
constexpr std::array< float, 4u > transparent{ 0.0f, 0.0f, 0.0f, 0.0f };
}

#endif

