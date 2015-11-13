// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_EDK_SYSTEM_UNIQUE_IDENTIFIER_H_
#define MOJO_EDK_SYSTEM_UNIQUE_IDENTIFIER_H_

#include <stdint.h>
#include <string.h>

#include <functional>
#include <string>

#include "mojo/public/cpp/system/macros.h"

namespace mojo {

namespace embedder {
class PlatformSupport;
}  // namespace embedder

namespace system {

// |UniqueIdentifier| is a POD class whose value is used to uniquely identify
// things.
class UniqueIdentifier {
 public:
  // This generates a new identifier. Uniqueness is "guaranteed" (i.e.,
  // probabilistically) for identifiers.
  static UniqueIdentifier Generate(embedder::PlatformSupport* platform_support);

  // This reconstitutes an identifier from its representation as a string
  // (generated by |ToString()|, below). On success, |*success| is set to true;
  // on failure, it is set to false (and the return value is undefined).
  static UniqueIdentifier FromString(const std::string& s, bool* success);

  // Yields a string representation of this identifier (which can be
  // reconstituted using |FromString()|. The result will consist of printable,
  // non-whitespace ASCII characters. It will *not* contain the following
  // characters: ", ', or \.
  std::string ToString() const;

  bool operator==(const UniqueIdentifier& other) const {
    return memcmp(data_, other.data_, sizeof(data_)) == 0;
  }
  bool operator!=(const UniqueIdentifier& other) const {
    return !operator==(other);
  }
  bool operator<(const UniqueIdentifier& other) const {
    return memcmp(data_, other.data_, sizeof(data_)) < 0;
  }

  size_t GetHashValue() const;

 private:
  friend std::hash<mojo::system::UniqueIdentifier>;

  explicit UniqueIdentifier() {}

  unsigned char data_[16];

  // Copying and assignment allowed.
};
static_assert(sizeof(UniqueIdentifier) == 16,
              "UniqueIdentifier has wrong size.");
// We want to be able to take any buffer and cast it to a |UniqueIdentifier|.
static_assert(MOJO_ALIGNOF(UniqueIdentifier) == 1,
              "UniqueIdentifier requires nontrivial alignment.");

}  // namespace system
}  // namespace mojo

namespace std {

// Specialization of |std::hash<>| for |UniqueIdentifier|s, so they can be used
// in unordered sets/maps.
template <>
struct hash<mojo::system::UniqueIdentifier> {
  size_t operator()(mojo::system::UniqueIdentifier unique_identifier) const {
    return unique_identifier.GetHashValue();
  }
};

}  // namespace std

#endif  // MOJO_EDK_SYSTEM_UNIQUE_IDENTIFIER_H_
