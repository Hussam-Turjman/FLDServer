// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_STRING_VALUE_SERIALIZER_H_
#define BASE_JSON_JSON_STRING_VALUE_SERIALIZER_H_

#include "fldserver/base/macros.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/base/values.h"
#include "fldserver/fldserver_config.h"

#include <string>

class CORE_EXPORT JSONStringValueSerializer : public base::ValueSerializer
{
public:
    // |json_string| is the string that will be the destination of the
    // serialization.  The caller of the constructor retains ownership of the
    // string. |json_string| must not be null.
    explicit JSONStringValueSerializer(std::string* json_string);

    ~JSONStringValueSerializer() override;

    // Attempt to serialize the data structure represented by Value into
    // JSON.  If the return value is true, the result will have been written
    // into the string passed into the constructor.
    bool
    Serialize(const base::Value& root) override;

    // Equivalent to Serialize(root) except binary values are omitted from the
    // output.
    bool
    SerializeAndOmitBinaryValues(const base::Value& root);

    void
    set_pretty_print(bool new_value)
    {
        pretty_print_ = new_value;
    }
    bool
    pretty_print()
    {
        return pretty_print_;
    }

private:
    bool
    SerializeInternal(const base::Value& root, bool omit_binary_values);

    // Owned by the caller of the constructor.
    std::string* json_string_;
    bool pretty_print_;  // If true, serialization will span multiple lines.

    DISALLOW_COPY_AND_ASSIGN(JSONStringValueSerializer);
};

class CORE_EXPORT JSONStringValueDeserializer : public base::ValueDeserializer
{
public:
    // This retains a reference to the contents of |json_string|, so the data
    // must outlive the JSONStringValueDeserializer. |options| is a bitmask of
    // JSONParserOptions.
    explicit JSONStringValueDeserializer(const base::StringPiece& json_string, int options = 0);

    ~JSONStringValueDeserializer() override;

    // Attempt to deserialize the data structure encoded in the string passed
    // in to the constructor into a structure of Value objects.  If the return
    // value is null, and if |error_code| is non-null, |error_code| will
    // contain an integer error code (a JsonParseError in this case).
    // If |error_message| is non-null, it will be filled in with a formatted
    // error message including the location of the error if appropriate.
    // The caller takes ownership of the returned value.
    std::unique_ptr<base::Value>
    Deserialize(int* error_code, std::string* error_message) override;

private:
    // Data is owned by the caller of the constructor.
    base::StringPiece json_string_;
    const int options_;

    DISALLOW_COPY_AND_ASSIGN(JSONStringValueDeserializer);
};

#endif  // BASE_JSON_JSON_STRING_VALUE_SERIALIZER_H_