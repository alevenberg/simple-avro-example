#include "avro/Compiler.hh"
#include "avro/DataFile.hh"
#include "avro/Decoder.hh"
#include "avro/Stream.hh"
#include "avro/ValidSchema.hh"
#include "avro/GenericDatum.hh"
#include "avro/Generic.hh"
#include "schema1.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) try {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <avro-file>\n";
    return 1;
  }

  std::string const avro_file = argv[1];

  // Create a valid schema from reading a file.
  std::ifstream ifs(avro_file);
  avro::ValidSchema schema;
  avro::compileJsonSchema(ifs, schema);

  // Encode into a file.
  std::unique_ptr<avro::OutputStream> out = avro::fileOutputStream("test.bin");
  avro::EncoderPtr e = avro::binaryEncoder();
  e->init(*out);
  v1::State state;
  state.name = "New York";
  state.post_abbr = "NY";
  avro::encode(*e, state);
  out->flush();

  // Decode the file.
  std::unique_ptr<avro::InputStream> in = avro::fileInputStream("test.bin");
  avro::DecoderPtr d = avro::validatingDecoder(schema, avro::binaryDecoder());
  d->init(*in);

  v1::State decoded_state;
  avro::decode(*d, decoded_state);
  std::cout << "Name: " << decoded_state.name << "\n"<< "Post Abbr: " << decoded_state.post_abbr  << std::endl;
  return 0; 
} catch (const std::exception& error) {
  std::cout << error.what() << std::endl;
}
