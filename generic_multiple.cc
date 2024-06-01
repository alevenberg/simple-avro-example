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
  for (auto i = 0; i < 100; ++i) {
    v1::State state;
    state.name = "New York";
    state.post_abbr = "NY";
    avro::encode(*e, state);
  }
  out->flush();

  // Decode the file.
  std::unique_ptr<avro::InputStream> in = avro::fileInputStream("test.bin");
  avro::DecoderPtr d = avro::validatingDecoder(schema, avro::binaryDecoder());
  d->init(*in);

  for (auto i = 0; i < 100; ++i) {
    avro::GenericDatum datum(schema);
    avro::decode(*d, datum);
    std::cout << "Type: " << datum.type() << std::endl;
    if (datum.type() == avro::AVRO_RECORD) {
        const avro::GenericRecord& r = datum.value<avro::GenericRecord>();
        std::cout << "Field-count: " << r.fieldCount() << std::endl;
        if (r.fieldCount() == 2) {
            const avro::GenericDatum& f0 = r.fieldAt(0);
            if (f0.type() == avro::AVRO_STRING) {
                std::cout << "Name: " << f0.value<std::string>() << std::endl;
            }
            const avro::GenericDatum& f1 = r.fieldAt(1);
            if (f1.type() == avro::AVRO_STRING) {
                std::cout << "Post Abbr: " << f1.value<std::string>() << std::endl;
            }
        }
    }
  }
  
  return 0; 
} catch (const std::exception& error) {
  std::cout << error.what() << std::endl;
}
