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
#include <iomanip>

int main(int argc, char* argv[]) try {
  // Create a valid schema from reading a file.
  std::ifstream ifs("schema.avsc");
  avro::ValidSchema schema;
  avro::compileJsonSchema(ifs, schema);

  // Decode the file.
  std::ifstream f("out.bin",std::ios::binary);
  std::stringstream s;
  if (f) {
      s << f.rdbuf();    
      f.close();
  }  
  std::unique_ptr<avro::InputStream> in = avro::istreamInputStream(s);
  avro::DecoderPtr d = avro::validatingDecoder(schema, avro::binaryDecoder());
  d->init(*in);

  for (auto i = 0; i < 10; ++i) {
    std::cout << "Row " << i << " ";
    avro::GenericDatum datum(schema);
    avro::decode(*d, datum);
    if (datum.type() == avro::AVRO_RECORD) {
      const avro::GenericRecord& r = datum.value<avro::GenericRecord>();
      std::cout << "(" << r.fieldCount() << "): ";
      for (auto i = 0; i < r.fieldCount(); i++) {
        const avro::GenericDatum& datum = r.fieldAt(i);

        switch (datum.type()) {
          case avro::AVRO_STRING:
            std::cout << std::left << std::setw(15)
                      << datum.value<std::string>();
            break;
          case avro::AVRO_INT:
            std::cout << std::left << std::setw(15) << datum.value<int>();
            break;
          case avro::AVRO_LONG:
            std::cout << std::left << std::setw(15) << datum.value<long>();
            break;
            // Depending on the table you are reading, you might need to add
            // cases for other datatypes here.
          default:
            std::cout << std::left << std::setw(15) << "UNDEFINED " << " ";
        }
      }
    }
    std::cout << "\n";
  }


  return 0; 
} catch (const std::exception& error) {
  std::cout << error.what() << std::endl;
}
