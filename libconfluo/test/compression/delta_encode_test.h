#ifndef CONFLUO_TEST_DELTA_ENCODE_TEST_H_
#define CONFLUO_TEST_DELTA_ENCODE_TEST_H_

#include "compression/delta_encoder.h"
#include "compression/delta_decoder.h"
#include "container/bitmap/delta_encoded_array.h"
#include "gtest/gtest.h"

using namespace confluo;
using namespace confluo::compression;

class DeltaEncodeTest : public testing::Test {
 public:
  const uint64_t kArraySize = (1024ULL * 1024ULL);  // 1 KBytes
};

TEST_F(DeltaEncodeTest, DecodeFullTest) {
  size_t k_array_size = 1000;

  uint64_t *array = new uint64_t[k_array_size];
  for (uint64_t i = 0; i < k_array_size; i++) {
    array[i] = i % 100;
  }

  uint8_t* encode_buffer = new uint8_t[delta_encoder::get_buffer_size(array, k_array_size)];
  uint64_t* dest_buffer = new uint64_t[k_array_size];

  delta_encoder::encode(array, k_array_size, encode_buffer);
  delta_decoder::decode(encode_buffer, dest_buffer, k_array_size);

  for (size_t i = 0; i < k_array_size; i++) {
    ASSERT_EQ(array[i], dest_buffer[i]);
  }

  delete[] array;
  delete[] encode_buffer;
  delete[] dest_buffer;
}

TEST_F(DeltaEncodeTest, DecodePartialTest) {
  size_t k_array_size = 1000;

  uint64_t *array = new uint64_t[k_array_size];
  for (size_t i = 0; i < k_array_size; i++) {
    array[i] = i;
  }

  size_t src_index = 250;
  size_t buffer_size = 600;

  uint8_t* encode_buffer = new uint8_t[delta_encoder::get_buffer_size(array, k_array_size)];
  
  uint64_t* dest_buffer = new uint64_t[buffer_size];

  delta_encoder::encode(array, k_array_size, encode_buffer);
  delta_decoder::decode(encode_buffer, dest_buffer, src_index, buffer_size);

  for (size_t i = 0; i < buffer_size; i++) {
    ASSERT_EQ(array[i + src_index], dest_buffer[i]);
  }

  delete[] array;
  delete[] encode_buffer;
  delete[] dest_buffer;
}

TEST_F(DeltaEncodeTest, DecodePtrIndexTest) {
  size_t k_array_size = 1000;

  uint64_t *array = new uint64_t[k_array_size];
  for (size_t i = 0; i < k_array_size; i++) {
    array[i] = i;
  }

  size_t src_index = 250;
  size_t buffer_size = k_array_size - src_index;

  uint8_t* encode_buffer = new uint8_t[delta_encoder::get_buffer_size(array, k_array_size)];
  
  uint64_t* dest_buffer = new uint64_t[buffer_size];


  delta_encoder::encode(array, k_array_size, encode_buffer);
  delta_decoder::decode(encode_buffer, src_index, k_array_size,
          dest_buffer);

  for (size_t i = 0; i < buffer_size; i++) {
    ASSERT_EQ(array[i + src_index], dest_buffer[i]);
  }

  delete[] array;
  delete[] encode_buffer;
  delete[] dest_buffer;
}

TEST_F(DeltaEncodeTest, DecodeIndexTest) {
  size_t k_array_size = 1000;

  uint64_t *array = new uint64_t[k_array_size];
  for (size_t i = 0; i < k_array_size; i++) {
    array[i] = i;
  }

  uint8_t* encode_buffer = new 
      uint8_t[delta_encoder::get_buffer_size(array, k_array_size)];

  size_t src_index = 250;
  delta_encoder::encode(array, k_array_size, encode_buffer);
  uint8_t decoded_val = delta_decoder::decode(encode_buffer, src_index);

  ASSERT_EQ(array[src_index], decoded_val);

  delete[] array;
  delete[] encode_buffer;
}



#endif /* CONFLUO_TEST_DELTA_ENCODE_TEST_H_ */
