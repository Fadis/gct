#include <nnef.h>
#include <nlohmann/json.hpp>
#include <gct/dnn/nnef2json.hpp>

namespace nnef {

void to_json( nlohmann::json &dest, const nnef::Value &src ) {
  dest = nlohmann::json::object();
  if( src.kind() == nnef::Value::Kind::None ) {
    dest[ "type" ] = "None";
    dest[ "value" ] = nullptr;
  }
  else if( src.kind() == nnef::Value::Kind::Integer ) {
    dest[ "type" ] = "Integer";
    dest[ "value" ] = src.integer();
  }
  else if( src.kind() == nnef::Value::Kind::Scalar ) {
    dest[ "type" ] = "Scalar";
    dest[ "value" ] = src.scalar();
  }
  else if( src.kind() == nnef::Value::Kind::Logical ) {
    dest[ "type" ] = "Logical";
    dest[ "value" ] = src.logical();
  }
  else if( src.kind() == nnef::Value::Kind::String ) {
    dest[ "type" ] = "String";
    dest[ "value" ] = src.string();
  }
  else if( src.kind() == nnef::Value::Kind::Identifier ) {
    dest[ "type" ] = "Identifier";
    dest[ "value" ] = src.identifier();
  }
  else if( src.kind() == nnef::Value::Kind::Array ) {
    dest[ "type" ] = "Array";
    dest[ "value" ] = nlohmann::json::array();
    for( const auto &v: src.array() ) {
      nlohmann::json temp;
      to_json( temp, v );
      dest[ "value" ].push_back( temp );
    }
  }
  else if( src.kind() == nnef::Value::Kind::Tuple ) {
    dest[ "type" ] = "Tuple";
    dest[ "value" ] = nlohmann::json::array();
    for( const auto &v: src.tuple() ) {
      nlohmann::json temp;
      to_json( temp, v );
      dest[ "value" ].push_back( temp );
    }
  }
}

void to_json( nlohmann::json &dest, const nnef::ValueDict &src ) {
  dest = nlohmann::json::array();
  for( const auto &v: src ) {
    nlohmann::json temp = nlohmann::json::object();
    temp[ "key" ] = v.first;
    temp[ "value" ] = nlohmann::json( v.second );
    dest.push_back( temp );
  }
}

void to_json( nlohmann::json &dest, const nnef::Tensor &src ) {
  dest = nlohmann::json::object();
  dest[ "name" ] = src.name;
  dest[ "dtype" ] = src.dtype;
  dest[ "shape" ] = nlohmann::json::array();
  for( const auto &v: src.shape ) {
    dest[ "shape" ].push_back( v );
  }
  /*dest[ "data" ] = nlohmann::json::array();
  for( const auto &v: src.data ) {
    dest[ "data" ].push_back( int( v ) );
  }*/
  dest[ "quantization" ] = src.quantization;
}

void to_json( nlohmann::json &dest, const nnef::Operation &src ) {
  dest = nlohmann::json::object();
  dest[ "name" ] = src.name;
  dest[ "dtype" ] = src.dtype;
  dest[ "attribs" ] = src.attribs;
  dest[ "inputs" ] = src.inputs;
  dest[ "outputs" ] = src.outputs;
}

void to_json( nlohmann::json &dest, const nnef::Graph &src ) {
  dest = nlohmann::json::object();
  dest[ "name" ] = src.name;
  dest[ "tensors" ] = nlohmann::json::array();
  for( const auto &v: src.tensors ) {
    nlohmann::json temp = nlohmann::json::object();
    temp[ "key" ] = v.first;
    temp[ "value" ] = v.second;
    dest[ "tensors" ].push_back( temp );
  }
  dest[ "operations" ] = nlohmann::json::array();
  for( const auto &v: src.operations ) {
    dest[ "operations" ].push_back( v );
  }
  dest[ "inputs" ] = nlohmann::json::array();
  for( const auto &v: src.inputs ) {
    dest[ "inputs" ].push_back( v );
  }
  dest[ "outputs" ] = nlohmann::json::array();
  for( const auto &v: src.outputs ) {
    dest[ "outputs" ].push_back( v );
  }
}

}

