/* @cpp.file.header */

/*  _________        _____ __________________        _____
 *  __  ____/___________(_)______  /__  ____/______ ____(_)_______
 *  _  / __  __  ___/__  / _  __  / _  / __  _  __ `/__  / __  __ \
 *  / /_/ /  _  /    _  /  / /_/ /  / /_/ /  / /_/ / _  /  _  / / /
 *  \____/   /_/     /_/   \_,__/   \____/   \__,_/  /_/   /_/ /_/
 */

#ifndef GRIDCLIENT_PORTABLE_MARSHALLER_HPP_INCLUDED
#define GRIDCLIENT_PORTABLE_MARSHALLER_HPP_INCLUDED

#include <iterator>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/detail/endian.hpp>

#include "gridgain/gridportable.hpp"
#include "gridgain/gridclienthash.hpp"
#include "gridgain/gridportablereader.hpp"
#include "gridgain/gridportablewriter.hpp"
#include "gridgain/impl/utils/gridclientbyteutils.hpp"
#include "gridgain/impl/marshaller/portable/gridclientportablemessages.hpp"
#include "gridgain/impl/cmd/gridclientmessagetopologyresult.hpp"
#include "gridgain/impl/cmd/gridclientmessagelogresult.hpp"
#include "gridgain/impl/cmd/gridclientmessagecacheresult.hpp"
#include "gridgain/impl/cmd/gridclientmessagecachemodifyresult.hpp"
#include "gridgain/impl/cmd/gridclientmessagecachemetricsresult.hpp"
#include "gridgain/impl/cmd/gridclientmessagecachegetresult.hpp"
#include "gridgain/impl/cmd/gridclientmessagetaskresult.hpp"

const int8_t TYPE_ID_BYTE = 1;
const int8_t TYPE_ID_SHORT = 2;
const int8_t TYPE_ID_INT = 3;
const int8_t TYPE_ID_LONG = 4;
const int8_t TYPE_ID_FLOAT = 5;
const int8_t TYPE_ID_DOUBLE = 6;
const int8_t TYPE_ID_CHAR = 7;
const int8_t TYPE_ID_BOOLEAN = 8;
const int8_t TYPE_ID_STRING = 9;
const int8_t TYPE_ID_UUID = 10;
const int8_t TYPE_ID_DATE = 11;
const int8_t TYPE_ID_BYTE_ARR = 12;
const int8_t TYPE_ID_SHORT_ARR = 13;
const int8_t TYPE_ID_INT_ARR = 14;
const int8_t TYPE_ID_LONG_ARR = 15;
const int8_t TYPE_ID_FLOAT_ARR = 16;
const int8_t TYPE_ID_DOUBLE_ARR = 17;
const int8_t TYPE_ID_CHAR_ARR = 18;
const int8_t TYPE_ID_BOOLEAN_ARR = 19;
const int8_t TYPE_ID_STRING_ARR = 20;
const int8_t TYPE_ID_UUID_ARR = 21;
const int8_t TYPE_ID_DATE_ARR = 22;
const int8_t TYPE_ID_OBJ_ARR = 23;
const int8_t TYPE_ID_COLLECTION = 24;
const int8_t TYPE_ID_MAP = 25;
const int8_t TYPE_ID_PORTABLE = 26;

const int8_t FLAG_NULL = 101;
const int8_t FLAG_HANDLE = 102;
const int8_t FLAG_OBJECT = 103;

GridPortable* createPortable(int32_t typeId, GridPortableReader& reader);

bool systemPortable(int32_t typeId);

GridPortable* createSystemPortable(int32_t typeId, GridPortableReader& reader);

int32_t getFieldId(const char* fieldName, int32_t typeId, GridPortableIdResolver* idRslvr);

int32_t getFieldId(const std::string& fieldName, int32_t typeId, GridPortableIdResolver* idRslvr);

class PortableReadContext {
private:
    PortableReadContext(const PortableReadContext& other) {
    }

public:
    PortableReadContext(const boost::shared_ptr<std::vector<int8_t>>& dataPtr, GridPortableIdResolver* idRslvr) :
        dataPtr(dataPtr), idRslvr(idRslvr) {
    }

    boost::unordered_map<int32_t, void*> handles;

    boost::shared_ptr<std::vector<int8_t>> dataPtr;

    GridPortableIdResolver* idRslvr;
};

#ifdef BOOST_BIG_ENDIAN
class PortableOutput {
public:
    PortableOutput(size_t cap) {
        bytes.reserve(cap);
    }

    virtual ~PortableOutput() {
    }

    void writeBool(bool val) {
        writeByte(val ? 1 : 0);
    }

    void writeByte(int8_t val) {
        bytes.push_back(val);
    }

    void writeBytes(const void* src, size_t size) {
        const int8_t* ptr = reinterpret_cast<const int8_t*>(src);

        bytes.insert(bytes.end(), ptr, ptr + size);
    }

    void writeInt16Array(const std::vector<int16_t>& val) {
        for (auto iter = val.begin(); iter != val.end(); ++iter)
            writeInt16(*iter);
    }

    void writeInt16Array(const int16_t* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeInt16(val[i]);
    }

    void writeInt32Array(const std::vector<int32_t>& val) {
        for (auto iter = val.begin(); iter != val.end(); ++iter)
            writeInt32(*iter);
    }

    void writeInt32Array(const int32_t* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeInt32(val[i]);
    }

    void writeCharArray(const std::vector<uint16_t>& val) {
        for (auto iter = val.begin(); iter != val.end(); ++iter)
            writeChar(*iter);
    }

    void writeCharArray(const uint16_t* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeChar(val[i]);
    }

    void writeInt64Array(const std::vector<int64_t>& val) {
        for (auto iter = val.begin(); iter != val.end(); ++iter)
            writeInt64(*iter);
    }

    void writeInt64Array(const int64_t* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeInt64(val[i]);
    }

    void writeFloatArray(const std::vector<float>& val) {
        for (auto iter = val.begin(); iter != val.end(); ++iter)
            writeFloat(*iter);
    }

    void writeFloatArray(const float* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeFloat(val[i]);
    }

    void writeDoubleArray(const std::vector<double>& val) {
        for (auto iter = val.begin(); iter != val.end(); ++iter)
            writeDouble(*iter);
    }

    void writeDoubleArray(const double* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeDouble(val[i]);
    }

    void writeBoolArray(const std::vector<bool>& val) {
        for (int32_t i = 0; i < val.size(); i++)
            writeByte(val[i] ? 1 : 0);
    }

    void writeBoolArray(const bool* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeByte(val[i] ? 1 : 0);
    }

    void writeInt16(int16_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        size_t size = bytes.size();

        bytes.resize(size + 2);

        std::reverse_copy(ptr, ptr + 2, bytes.data() + size);
    }

    void writeChar(uint16_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        size_t size = bytes.size();

        bytes.resize(size + 2);

        std::reverse_copy(ptr, ptr + 2, bytes.data() + size);
    }

    void writeInt32(int32_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        size_t size = bytes.size();

        bytes.resize(size + 4);

        std::reverse_copy(ptr, ptr + 4, bytes.data() + size);
    }

    void writeInt32(int32_t val, int32_t pos) {
        assert(pos < bytes.size());

        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        int8_t* dst = reinterpret_cast<int8_t*>(bytes.data());

        std::reverse_copy(ptr, ptr + 4, dst + pos);
    }

    void writeInt64(int64_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        size_t size = bytes.size();

        bytes.resize(size + 8);

        std::reverse_copy(ptr, ptr + 8, bytes.data() + size);
    }

    void writeFloat(float val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        size_t size = bytes.size();

        bytes.resize(size + 4);

        std::reverse_copy(ptr, ptr + 4, bytes.data() + size);
    }

    void writeDouble(double val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        size_t size = bytes.size();

        bytes.resize(size + 8);

        std::reverse_copy(ptr, ptr + 8, bytes.data() + size);
    }

    std::vector<int8_t> bytes;
};
#else
class PortableOutput {
public:
    PortableOutput(std::vector<int8_t>& bytes) : bytes(bytes) {
    }

    virtual ~PortableOutput() {
    }

    void writeBool(bool val) {
        writeByte(val ? 1 : 0);
    }

    void writeByte(int8_t val) {
        bytes.push_back(val);
    }

	void writeBytes(const void* src, size_t size) {
        const int8_t* ptr = reinterpret_cast<const int8_t*>(src);

        bytes.insert(bytes.end(), ptr, ptr + size);
	}

    void writeInt16Array(const std::vector<int16_t>& val) {
        writeBytes(val.data(), val.size() * 2);
    }

    void writeInt16Array(const int16_t* val, int32_t size) {
        writeBytes(val, size * 2);
    }

    void writeInt32Array(const std::vector<int32_t>& val) {
        writeBytes(val.data(), val.size() * 4);
    }

    void writeInt32Array(const int32_t* val, int32_t size) {
        writeBytes(val, size * 4);
    }

    void writeCharArray(const std::vector<uint16_t>& val) {
        writeBytes(val.data(), val.size() * 2);
    }

    void writeCharArray(const uint16_t* val, int32_t size) {
        writeBytes(val, size * 2);
    }

    void writeInt64Array(const std::vector<int64_t>& val) {
        writeBytes(val.data(), val.size() * 8);
    }

    void writeInt64Array(const int64_t* val, int32_t size) {
        writeBytes(val, size * 8);
    }

    void writeFloatArray(const std::vector<float>& val) {
        writeBytes(val.data(), val.size() * 4);
    }

    void writeFloatArray(const float* val, int32_t size) {
        writeBytes(val, size * 4);
    }

    void writeDoubleArray(const std::vector<double>& val) {
        writeBytes(val.data(), val.size() * 2);
    }

    void writeDoubleArray(const double* val, int32_t size) {
        writeBytes(val, size * 8);
    }

    void writeBoolArray(const std::vector<bool>& val) {
        for (size_t i = 0; i < val.size(); i++)
            writeByte(val[i] ? 1 : 0);
    }

    void writeBoolArray(const bool* val, int32_t size) {
        for (int32_t i = 0; i < size; i++)
            writeByte(val[i] ? 1 : 0);
    }

	void writeInt16(int16_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        bytes.insert(bytes.end(), ptr, ptr + 2);
	}

    void writeChar(uint16_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        bytes.insert(bytes.end(), ptr, ptr + 2);
    }

    void writeInt32To(int32_t pos, int32_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        int8_t* dst = reinterpret_cast<int8_t*>(bytes.data());

        std::copy(ptr, ptr + 4, dst + pos);
    }

	void writeInt32(int32_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        bytes.insert(bytes.end(), ptr, ptr + 4);
	}

    void writeInt64(int64_t val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        bytes.insert(bytes.end(), ptr, ptr + 8);
	}

    void writeFloat(float val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        bytes.insert(bytes.end(), ptr, ptr + 4);
	}

    void writeDouble(double val) {
        int8_t* ptr = reinterpret_cast<int8_t*>(&val);

        bytes.insert(bytes.end(), ptr, ptr + 8);
	}

    std::vector<int8_t>& bytes;
};
#endif

class WriteContext {
public:
    WriteContext(std::vector<int8_t>& bytes, GridPortableIdResolver* idRslvr) : out(bytes), idRslvr(idRslvr) {
    }

    PortableOutput out;

    GridPortableIdResolver* idRslvr;

    boost::unordered_map<void*, int32_t> handles;

    int32_t lookup(void* ptr, int32_t off) {
        boost::unordered_map<void*, int32_t>::const_iterator handle = handles.find(ptr);

        if (handle == handles.end()) {
            handles[ptr] = off;

            return -1;
        }
        else
            return (*handle).second;
    }
};

const int32_t TOTAL_LENGTH_POS = 10;
const int32_t RAW_DATA_OFF_POS = 14;

class GridPortableWriterImpl : public GridPortableWriter, public GridPortableRawWriter {
public:
	int32_t curTypeId;

    std::map<int32_t, char*> fieldIds;

    GridPortableWriterImpl(WriteContext& ctx, int32_t curTypeId, int32_t objStart) : ctx(ctx), start(objStart), allowFields(true), curTypeId(curTypeId) {
    }

    virtual ~GridPortableWriterImpl() {
    }

    void writePortable(GridPortable& portable) {
        doWriteVariant(GridClientVariant(&portable));
    }

    GridPortableRawWriter& rawWriter() {
        return *this;
    }

    void writeFieldName(char* fieldName) {
        if (!allowFields)
            throw GridClientPortableException("Named fields are not allowed after raw data.");

        int32_t fieldId = getFieldId(fieldName, curTypeId, ctx.idRslvr);

        std::pair<std::map<int32_t, char*>::iterator, bool> add = fieldIds.insert(std::pair<int32_t, char*>(fieldId, fieldName));

        if (!add.second) {
            std::pair<int32_t, char*> pair = *add.first;

            std::ostringstream msg;

            msg << "Field id conflict [name1=" << pair.second << ", name2=" << fieldName << ", id=" << fieldId << "]";

            throw GridClientPortableException(msg.str());
        }

        ctx.out.writeInt32(fieldId);
    }

    void switchToRaw() {
        if (allowFields) {
            ctx.out.writeInt32To(start + RAW_DATA_OFF_POS, ctx.out.bytes.size() - start);

            allowFields = false;
        }
    }

    void writeByte(char* fieldName, int8_t val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(2);
        ctx.out.writeByte(TYPE_ID_BYTE);

        doWriteByte(val);
	}

    void doWriteByte(int8_t val) {
        ctx.out.writeByte(val);
	}

    void writeInt16(char* fieldName, int16_t val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(3);
        ctx.out.writeByte(TYPE_ID_SHORT);

        doWriteInt16(val);
	}

    void doWriteInt16(int16_t val) {
        ctx.out.writeInt16(val);
	}

    void writeChar(char* fieldName, uint16_t val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(3);
        ctx.out.writeByte(TYPE_ID_CHAR);

        doWriteInt16(val);
    }

    void doWriteChar(uint16_t val) {
        ctx.out.writeChar(val);
	}

    void writeCharArray(char* fieldName, const uint16_t* val, int32_t size) {
		writeFieldName(fieldName);

        if (val) {
            ctx.out.writeInt32(5 + size * 2);
            ctx.out.writeByte(TYPE_ID_CHAR_ARR);

            ctx.out.writeInt32(size);
            ctx.out.writeCharArray(val, size);
        }
        else {
            ctx.out.writeInt32(5 + size * 2);
            ctx.out.writeByte(TYPE_ID_CHAR_ARR);

            ctx.out.writeInt32(-1);
        }
    }

    void writeInt16Array(char* fieldName, const int16_t* val, int32_t size) {
		writeFieldName(fieldName);

        if (val) {
            ctx.out.writeInt32(5 + size * 2);
            ctx.out.writeByte(TYPE_ID_SHORT_ARR);

            ctx.out.writeInt32(size);
            ctx.out.writeInt16Array(val, size);
        }
        else {
            ctx.out.writeInt32(5);
            ctx.out.writeByte(TYPE_ID_SHORT_ARR);

            ctx.out.writeInt32(-1);
        }
    }

    void writeInt32(char* fieldName, int32_t val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(5);
        ctx.out.writeByte(TYPE_ID_INT);

        doWriteInt32(val);
	}

    void doWriteInt32(int32_t val) {
        ctx.out.writeInt32(val);
	}

    void writeInt32Array(char* fieldName, const int32_t* val, int32_t size) {
		writeFieldName(fieldName);

        if (val) {
            ctx.out.writeInt32(5 + size * 4);
            ctx.out.writeByte(TYPE_ID_INT_ARR);

            ctx.out.writeInt32(size);
            ctx.out.writeInt32Array(val, size);
        }
        else {
            ctx.out.writeInt32(5);
            ctx.out.writeByte(TYPE_ID_INT_ARR);

            ctx.out.writeInt32(-1);
        }
    }

    void writeInt64(char* fieldName, int64_t val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(9);
        ctx.out.writeByte(TYPE_ID_LONG);

        doWriteInt64(val);
	}

    void doWriteInt64(int64_t val) {
        ctx.out.writeInt64(val);
	}

    void writeInt64Array(char* fieldName, const int64_t* val, int32_t size) {
		writeFieldName(fieldName);

        if (val) {
            ctx.out.writeInt32(5 + size * 8);
            ctx.out.writeByte(TYPE_ID_LONG_ARR);

            ctx.out.writeInt32(size);
            ctx.out.writeInt64Array(val, size);
        }
        else {
            ctx.out.writeInt32(5);
            ctx.out.writeByte(TYPE_ID_LONG_ARR);

            ctx.out.writeInt32(-1);
        }
    }

    void writeFloat(char* fieldName, float val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(5);
        ctx.out.writeByte(TYPE_ID_FLOAT);

        doWriteFloat(val);
	}

    void doWriteFloat(float val) {
        ctx.out.writeFloat(val);
	}

    void writeFloatArray(char* fieldName, const float* val, int32_t size) {
		writeFieldName(fieldName);

        if (val) {
            ctx.out.writeInt32(5 + size * 4);
            ctx.out.writeByte(TYPE_ID_FLOAT_ARR);

            ctx.out.writeInt32(size);
            ctx.out.writeFloatArray(val, size);
        }
        else {
            ctx.out.writeInt32(5);
            ctx.out.writeByte(TYPE_ID_FLOAT_ARR);

            ctx.out.writeInt32(-1);
        }
    }

    void writeDouble(char* fieldName, double val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(9);
        ctx.out.writeByte(TYPE_ID_DOUBLE);

        doWriteDouble(val);
	}

    void doWriteDouble(double val) {
        ctx.out.writeDouble(val);
	}

    void writeDoubleArray(char* fieldName, const double* val, int32_t size) {
		writeFieldName(fieldName);

        if (val) {
            ctx.out.writeInt32(5 + size * 8);
            ctx.out.writeByte(TYPE_ID_DOUBLE_ARR);

            ctx.out.writeInt32(size);
            ctx.out.writeDoubleArray(val, size);
        }
        else {
            ctx.out.writeInt32(5);
            ctx.out.writeByte(TYPE_ID_DOUBLE_ARR);

            ctx.out.writeInt32(-1);
        }
    }

	void writeString(char* fieldName, const std::string& val) {
		writeFieldName(fieldName);

        int32_t len = val.length() * sizeof(char);
        ctx.out.writeInt32(5 + len);

        ctx.out.writeByte(TYPE_ID_STRING);

        ctx.out.writeInt32(len);
		ctx.out.writeBytes(val.data(), len);
	}

	void writeString(char* fieldName, const boost::optional<std::string>& val) {
		if (val)
            writeString(fieldName, val.get());
        else {
            writeFieldName(fieldName);

            ctx.out.writeInt32(5);

            ctx.out.writeByte(TYPE_ID_STRING);
            ctx.out.writeInt32(-1);
        }
	}

    void doWriteString(const std::string &val) {
        int32_t len = val.length() * sizeof(char);

        ctx.out.writeInt32(len);
		ctx.out.writeBytes(val.data(), len);
	}

    void writeWString(char* fieldName, const std::wstring& val) {
		writeFieldName(fieldName);

        int32_t len = val.length() * sizeof(wchar_t);
        ctx.out.writeInt32(5 + len);

        ctx.out.writeByte(TYPE_ID_STRING);

        ctx.out.writeInt32(len);
		ctx.out.writeBytes(val.data(), len);
    }

    void writeWString(char* fieldName, const boost::optional<std::wstring>& val) {
        if (val)
            writeWString(fieldName, val.get());
        else {
    		writeFieldName(fieldName);

            ctx.out.writeInt32(5);

            ctx.out.writeByte(TYPE_ID_STRING);
            ctx.out.writeInt32(-1);
        }
    }

    void doWriteWString(const std::wstring& str) {
        int32_t len = str.length() * sizeof(wchar_t);

        ctx.out.writeInt32(len);
		ctx.out.writeBytes(str.data(), len);
	}

    void writeByteArray(char* fieldName, const int8_t* val, int32_t size) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(val ? 5 + size : 5);

        ctx.out.writeByte(TYPE_ID_BYTE_ARR);

        doWriteByteArray(val, size);
    }

    void doWriteByteArray(const int8_t* val, int32_t size) {
        if (val) {
            ctx.out.writeInt32(size);
            ctx.out.writeBytes(val, size);
        }
        else
            ctx.out.writeInt32(-1);
    }

	void writeBool(char* fieldName, bool val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(2);
        ctx.out.writeByte(TYPE_ID_BOOLEAN);

        doWriteBool(val);
	}

	void doWriteBool(bool val) {
        ctx.out.writeBool(val);
	}

    void writeBoolArray(char* fieldName, const bool* val, int32_t size) {
		writeFieldName(fieldName);

        if (val) {
            ctx.out.writeInt32(size + 5);
            ctx.out.writeByte(TYPE_ID_BOOLEAN_ARR);

            ctx.out.writeInt32(size);

            for (int32_t i = 0; i < size; i++)
                doWriteBool(val[i]);
        }
        else {
            ctx.out.writeInt32(5);
            ctx.out.writeByte(TYPE_ID_BOOLEAN_ARR);

            ctx.out.writeInt32(-1);
        }
    }

	void writeUuid(char* fieldName, const GridClientUuid& val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(18);

        ctx.out.writeByte(TYPE_ID_UUID);

        doWriteUuid(val);
    }

	void writeUuid(char* fieldName, const boost::optional<GridClientUuid>& val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(val ? 18 : 2);

        ctx.out.writeByte(TYPE_ID_UUID);

        doWriteUuid(val);
	}

    void doWriteUuid(const boost::optional<GridClientUuid>& val) {
        if (val)
            doWriteUuid(val.get());
        else
            ctx.out.writeBool(false);
	}

    void doWriteUuid(const GridClientUuid& val) {
        ctx.out.writeBool(true);

        ctx.out.writeInt64(val.mostSignificantBits());
        ctx.out.writeInt64(val.leastSignificantBits());
	}

    void writeDate(char* fieldName, const boost::optional<GridClientDate>& val) {
		writeFieldName(fieldName);

        doWriteInt32(val ? 12 : 2);
        doWriteByte(TYPE_ID_DATE);

        if (val)
            doWriteDate(val.get());
        else
            doWriteBool(false);
    }

    void writeDate(char* fieldName, const GridClientDate& val) {
		writeFieldName(fieldName);

        doWriteInt32(12);
        doWriteByte(TYPE_ID_DATE);

        doWriteDate(val);
    }

    void doWriteDate(const GridClientDate& val) {
        doWriteBool(true);

        doWriteInt64(val.getTime());

        doWriteInt16(val.getNanoTicks());
    }

    int32_t startBoolArray() {
        return startArray(TYPE_ID_BOOLEAN_ARR);
    }

    int32_t startByteArray() {
        return startArray(TYPE_ID_BYTE_ARR);
    }

    int32_t startInt16Array() {
        return startArray(TYPE_ID_SHORT_ARR);
    }

    int32_t startCharArray() {
        return startArray(TYPE_ID_CHAR_ARR);
    }

    int32_t startInt32Array() {
        return startArray(TYPE_ID_INT_ARR);
    }

    int32_t startInt64Array() {
        return startArray(TYPE_ID_LONG_ARR);
    }

    int32_t startFloatArray() {
        return startArray(TYPE_ID_FLOAT_ARR);
    }

    int32_t startDoubleArray() {
        return startArray(TYPE_ID_DOUBLE_ARR);
    }

    int32_t startStringArray() {
        return startArray(TYPE_ID_STRING_ARR);
    }

    int32_t startUuidArray() {
        return startArray(TYPE_ID_UUID_ARR);
    }

    int32_t startDateArray() {
        return startArray(TYPE_ID_DATE_ARR);
    }

    int32_t startVariantArray() {
        return startArray(TYPE_ID_OBJ_ARR);
    }

    int32_t startVariantCollection() {
        return startArray(TYPE_ID_COLLECTION);
    }

    int32_t startArray(int8_t type) {
        ctx.out.writeInt32(0); // Reserve for field length.

        int32_t start = ctx.out.bytes.size();

        ctx.out.writeByte(type);

        ctx.out.writeInt32(0); // Reserve for array elements count.

        return start;
    }

    void endArray(int32_t start, int32_t cnt) {
        int32_t len = ctx.out.bytes.size() - start;

        ctx.out.writeInt32To(start - 4, len);

        ctx.out.writeInt32To(start + 1, cnt);
    }

    void writeHeader(bool userType, int32_t typeId, const GridClientVariant& val) {
        writeHeader(userType, typeId, val.hasPortable() ? 0 : val.hashCode());
    }

    void writeHeader(bool userType, int32_t typeId, int32_t hashCode) {
        ctx.out.writeBool(userType);
        ctx.out.writeInt32(typeId);
        ctx.out.writeInt32(hashCode);

        ctx.out.writeInt32(0); // Reserve space for length.

        ctx.out.writeInt32(18); // Raw offset (header length).
    }

    void writeVariant(char* fieldName, const GridClientVariant& val) {
		writeFieldName(fieldName);

        ctx.out.writeInt32(0); // Reserve space for length.

        int32_t fieldStart = ctx.out.bytes.size();

        doWriteVariant(val);

        int32_t len = ctx.out.bytes.size() - fieldStart;

        ctx.out.writeInt32To(fieldStart - 4, len);
    }

    void writeSystem(GridPortable& portable) {
        int32_t start = ctx.out.bytes.size();
        int32_t lenPos = start + 10;

        ctx.out.writeByte(FLAG_OBJECT);

        writeHeader(false, portable.typeId(), 0);

        GridPortableWriterImpl writer(ctx, portable.typeId(), ctx.out.bytes.size() - 18);

        portable.writePortable(writer);

        int32_t len = ctx.out.bytes.size() - start;

        ctx.out.writeInt32To(lenPos, len);

        writer.writeRawOffsetIfNeeded(start, len);
    }

    void writeRawOffsetIfNeeded(int32_t start, int32_t len) {
        if (allowFields)
            ctx.out.writeInt32To(start + 14, len);
    }

    template<class InputIterator, typename T>
    void doWriteArray(InputIterator first, InputIterator last) {
        int32_t start = ctx.out.bytes.size();

        ctx.out.writeInt32(0);

        int32_t cnt = 0;

        while (first != last) {
            const T& val = *first;

            writeArrayElement<T>(val);

            first++;

            cnt++;
        }

        ctx.out.writeInt32To(start, cnt);
    }

    void doWriteVariant(const GridClientVariant& val) {
        int32_t start = ctx.out.bytes.size();
        int32_t lenPos = start + 10;

        if (val.hasPortable() || val.hasHashablePortable()) {
            GridPortable* portable = val.hasPortable() ? val.getPortable() : val.getHashablePortable();

            int32_t handle = ctx.lookup(portable, ctx.out.bytes.size());

            if (handle >= 0) {
                handle = start - handle;

                ctx.out.writeByte(FLAG_HANDLE);

                ctx.out.writeInt32(handle);

                return;
            }
            else {
                ctx.out.writeByte(FLAG_OBJECT);

                writeHeader(true, portable->typeId(), val);

                GridPortableWriterImpl writer(ctx, portable->typeId(), ctx.out.bytes.size() - 18);

                portable->writePortable(writer);

                writer.writeRawOffsetIfNeeded(start, ctx.out.bytes.size() - start);
            }
        }
        else if (val.hasPortableObject()) {
            GridPortableObject& obj = val.getPortableObject();

            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_PORTABLE, val);

            std::vector<int8_t>* data = obj.data();

            ctx.out.writeInt32(data->size());
            ctx.out.writeBytes(data->data(), data->size());
            ctx.out.writeInt32(obj.start());
        }
        else if (val.hasByte()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_BYTE, val);

            doWriteByte(val.getByte());
        }
        else if (val.hasShort()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_SHORT, val);

            doWriteInt16(val.getShort());
        }
        else if (val.hasInt()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_INT, val);

            doWriteInt32(val.getInt());
        }
        else if (val.hasLong()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_LONG, val);

            doWriteInt64(val.getLong());
        }
        else if (val.hasFloat()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_FLOAT, val);

            doWriteFloat(val.getFloat());
        }
        else if (val.hasDouble()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_DOUBLE, val);

            doWriteDouble(val.getDouble());
        }
        else if (val.hasChar()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_CHAR, val);

            doWriteChar(val.getChar());
        }
        else if (val.hasString()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_STRING, val);

            doWriteString(val.getString());
        }
        else if (val.hasBool()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_BOOLEAN, val);

            doWriteBool(val.getBool());
        }
        else if (val.hasUuid()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_UUID, val);

            doWriteUuid(val.getUuid());
        }
        else if (val.hasDate()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_DATE, val);

            doWriteDate(val.getDate());
        }
        else if (val.hasByteArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_BYTE_ARR, val);

            std::vector<int8_t>& arr = val.getByteArray();

            doWriteByteArray(arr.data(), arr.size());
        }
        else if (val.hasShortArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_SHORT_ARR, val);

            std::vector<int16_t>& arr = val.getShortArray();

            ctx.out.writeInt32(arr.size());
            ctx.out.writeInt16Array(arr.data(), arr.size());
        }
        else if (val.hasIntArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_INT_ARR, val);

            std::vector<int32_t>& arr = val.getIntArray();

            ctx.out.writeInt32(arr.size());
            ctx.out.writeInt32Array(arr.data(), arr.size());
        }
        else if (val.hasLongArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_LONG_ARR, val);

            std::vector<int64_t>& arr = val.getLongArray();

            ctx.out.writeInt32(arr.size());
            ctx.out.writeInt64Array(arr.data(), arr.size());
        }
        else if (val.hasFloatArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_FLOAT_ARR, val);

            std::vector<float>& arr = val.getFloatArray();

            ctx.out.writeInt32(arr.size());
            ctx.out.writeFloatArray(arr.data(), arr.size());
        }
        else if (val.hasDoubleArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_DOUBLE_ARR, val);

            std::vector<double>& arr = val.getDoubleArray();

            ctx.out.writeInt32(arr.size());
            ctx.out.writeDoubleArray(arr.data(), arr.size());
        }
        else if (val.hasCharArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_CHAR_ARR, val);

            std::vector<uint16_t>& arr = val.getCharArray();

            ctx.out.writeInt32(arr.size());
            ctx.out.writeCharArray(arr.data(), arr.size());
        }
        else if (val.hasStringArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_STRING_ARR, val);

            std::vector<std::string>& arr = val.getStringArray();

            doWriteArray<std::vector<std::string>::iterator, std::string>(arr.begin(), arr.end());
        }
        else if (val.hasBoolArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_BOOLEAN_ARR, val);

            std::vector<bool>& arr = val.getBoolArray();

            ctx.out.writeInt32(arr.size());

            for (auto iter = arr.begin(); iter != arr.end(); ++iter)
                doWriteBool(*iter);
        }
        else if (val.hasUuidArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_UUID_ARR, val);

            std::vector<GridClientUuid>& arr = val.getUuidArray();

            doWriteArray<std::vector<GridClientUuid>::iterator, GridClientUuid>(arr.begin(), arr.end());
        }
        else if (val.hasDateArray()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_DATE_ARR, val);

            std::vector<GridClientDate>& arr = val.getDateArray();

            doWriteArray<std::vector<GridClientDate>::iterator, GridClientDate>(arr.begin(), arr.end());
        }
        else if (val.hasVariantVector()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_COLLECTION, val);

            doWriteVariantCollection(val.getVariantVector());
        }
        else if (val.hasVariantMap()) {
            ctx.out.writeByte(FLAG_OBJECT);

            writeHeader(false, TYPE_ID_MAP, val);

            doWriteVariantMap(val.getVariantMap());
        }
        else if (!val.hasAnyValue()) {
            ctx.out.writeByte(FLAG_NULL);

            return;
        }
        else {
            assert(false);

            throw GridClientPortableException("Unknown object type.");
        }

        int32_t len = ctx.out.bytes.size() - start;

        ctx.out.writeInt32To(lenPos, len);
    }

    void doWriteVariantCollection(const TGridClientVariantSet& col) {
        ctx.out.writeInt32(col.size());

        ctx.out.writeByte(static_cast<int8_t>(GridCollectionType::ARRAY_LIST));

        for (auto iter = col.begin(); iter != col.end(); ++iter) {
            const GridClientVariant& variant = *iter;

            doWriteVariant(variant);
        }
    }

    void writeVariantMap(char* fieldName, const TGridClientVariantMap &val) {
        writeFieldName(fieldName);

        ctx.out.writeInt32(0); // Reserve space for length.

        int32_t fieldStart = ctx.out.bytes.size();

        doWriteByte(TYPE_ID_MAP);

        doWriteVariantMap(val);

        int32_t len = ctx.out.bytes.size() - fieldStart;

        ctx.out.writeInt32To(fieldStart - 4, len);
    }

    void doWriteVariantMap(const TGridClientVariantMap& map) {
        ctx.out.writeInt32(map.size());

        ctx.out.writeByte(static_cast<int8_t>(GridMapType::HASH_MAP));

        for (auto iter = map.begin(); iter != map.end(); ++iter) {
            const GridClientVariant& key = iter->first;
            const GridClientVariant& val = iter->second;

            doWriteVariant(key);
            doWriteVariant(val);
        }
    }

    void writeByte(int8_t val) {
        switchToRaw();

        doWriteByte(val);
	}

    void writeInt16(int16_t val) {
        switchToRaw();

        doWriteInt16(val);
	}

    void writeInt16Array(const int16_t* val, int32_t size) {
        switchToRaw();

        if (val) {
            ctx.out.writeInt32(size);
            ctx.out.writeInt16Array(val, size);
        }
        else
            ctx.out.writeInt32(-1);
    }

    void writeInt32(int32_t val) {
        switchToRaw();

        doWriteInt32(val);
	}

    void writeInt32Array(const int32_t* val, int32_t size) {
        switchToRaw();

        if (val) {
            ctx.out.writeInt32(size);
            ctx.out.writeInt32Array(val, size);
        }
        else
            ctx.out.writeInt32(-1);
    }

    void writeChar(uint16_t val) {
        switchToRaw();

        doWriteChar(val);
    }

    void writeCharArray(const uint16_t* val, int32_t size) {
        switchToRaw();

        if (val) {
            ctx.out.writeInt32(size);
            ctx.out.writeCharArray(val, size);
        }
        else
            ctx.out.writeInt32(-1);
    }

    void writeInt64(int64_t val) {
        switchToRaw();

        doWriteInt64(val);
	}

    void writeInt64Array(const int64_t* val, int32_t size) {
        switchToRaw();

        if (val) {
            ctx.out.writeInt32(size);
            ctx.out.writeInt64Array(val, size);
        }
        else
            ctx.out.writeInt32(-1);
    }

    void writeFloat(float val) {
        switchToRaw();

        doWriteFloat(val);
	}

    void writeFloatArray(const float* val, int32_t size) {
        switchToRaw();

        if (val) {
            ctx.out.writeInt32(size);
            ctx.out.writeFloatArray(val, size);
        }
        else
            ctx.out.writeInt32(-1);
    }

    void writeDouble(double val) {
        switchToRaw();

        doWriteDouble(val);
	}

    void writeDoubleArray(const double* val, int32_t size) {
        switchToRaw();

        if (val) {
            ctx.out.writeInt32(size);
            ctx.out.writeDoubleArray(val, size);
        }
        else
            ctx.out.writeInt32(-1);
    }

	void writeString(const std::string& val) {
        switchToRaw();

        doWriteString(val);
	}

    void writeString(const boost::optional<std::string>& val) {
        switchToRaw();

        if (!val)
            doWriteInt32(-1);
        else
            doWriteString(val.get());
    }

    void writeWString(const std::wstring& val) {
        switchToRaw();

        doWriteWString(val);
    }

    void writeWString(const boost::optional<std::wstring>& val) {
        switchToRaw();

        if (!val)
            doWriteInt32(-1);
        else
            doWriteWString(val.get());
    }

    void writeByteArray(const int8_t* val, int32_t size) {
        switchToRaw();

        doWriteByteArray(val, size);
    }

	void writeBool(bool val) {
        switchToRaw();

        doWriteBool(val);
	}

    void writeBoolArray(const bool* val, int32_t size) {
        switchToRaw();

        if (val) {
            ctx.out.writeInt32(size);

            for (int i = 0; i < size; i++)
                doWriteBool(val[i]);
        }
        else
            ctx.out.writeInt32(-1);
    }

	void writeUuid(const GridClientUuid& val) {
        switchToRaw();

        doWriteUuid(val);
	}

    void writeUuid(const boost::optional<GridClientUuid>& val) {
        switchToRaw();

        if (val)
            doWriteUuid(val);
        else
            doWriteBool(false);
	}

    void writeDate(const GridClientDate& val) {
        switchToRaw();

        doWriteDate(val);
    }

    void writeDate(const boost::optional<GridClientDate>& val) {
        switchToRaw();

        if (val)
            doWriteDate(val.get());
        else
            doWriteBool(false);
    }

    void writeVariant(const GridClientVariant& val) {
        switchToRaw();

        doWriteVariant(val);
    }

    void writeVariantMap(const TGridClientVariantMap& val) {
        switchToRaw();

        doWriteVariantMap(val);
    }

    int32_t startArrayRaw() {
        switchToRaw();

        int32_t start = ctx.out.bytes.size();

        ctx.out.writeInt32(0);

        return start;
    }

    void endArrayRaw(int32_t start, int32_t cnt) {
        ctx.out.writeInt32To(start, cnt);
    }

private:
    int32_t start;

    bool allowFields;

    WriteContext& ctx;
};

#ifdef BOOST_BIG_ENDIAN
class PortableInput {
public:
    PortableInput(std::vector<int8_t>& data) : bytes(data), pos(0) {
    }

    virtual ~PortableInput() {
    }

    int8_t readByte() {
        checkAvailable(1);

        return bytes[pos++];
    }

    int8_t readByte(int32_t off) {
        checkAvailable(off, 1);

        return bytes[off];
    }

    bool readBool(int32_t off) {
        checkAvailable(off, 1);

        int8_t val = bytes[off];

        return val != 0;
    }

    int32_t readInt32(int32_t off) {
        checkAvailable(off, 4);

        int32_t res;

        int8_t* start = bytes.data() + off;
        int8_t* end = start + 4;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::reverse_copy(start, end, dst);

        return res;
    }

    std::vector<int8_t> readBytes(int32_t size) {
        checkAvailable(size);

        std::vector<int8_t> vec;

        vec.insert(vec.end(), bytes.data() + pos, bytes.data() + pos + size);

        pos += size;

        return vec;
    }

    int8_t* readBytesArray(int32_t size) {
        checkAvailable(size);

        int8_t* res = new int8_t[size];

        memcpy(res, bytes.data() + pos, size);

        pos += size;

        return res;
    }

    int16_t readInt16() {
        checkAvailable(2);

        int16_t res;

        int8_t* start = bytes.data() + pos;
        int8_t* end = start + 2;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::reverse_copy(start, end, dst);

        pos += 2;

        return res;
    }

    int32_t readInt32() {
        checkAvailable(4);

        int32_t res;

        int8_t* start = bytes.data() + pos;
        int8_t* end = start + 4;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::reverse_copy(start, end, dst);

        pos += 4;

        return res;
    }

    int64_t readInt64() {
        checkAvailable(8);

        int64_t res;

        int8_t* start = bytes.data() + pos;
        int8_t* end = start + 8;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::reverse_copy(start, end, dst);

        pos += 8;

        return res;
    }

    float readFloat() {
        checkAvailable(4);

        float res;

        int8_t* start = bytes.data() + pos;
        int8_t* end = start + 4;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::reverse_copy(start, end, dst);

        pos += 4;

        return res;
    }

    double readDouble() {
        checkAvailable(4);

        double res;

        int8_t* start = bytes.data() + pos;
        int8_t* end = start + 8;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::reverse_copy(start, end, dst);

        pos += 8;

        return res;
    }

protected:
    void checkAvailable(size_t cnt) {
        assert(pos + cnt <= bytes.size());
    }

    void checkAvailable(int32_t off, size_t cnt) {
        assert(off + pos + cnt <= bytes.size());
    }

    int32_t pos;

    std::vector<int8_t>& bytes;
};
#else
class PortableInput {
public:
    PortableInput(std::vector<int8_t>& data) : bytes(data) {
    }

    virtual ~PortableInput() {
    }

    int8_t readByte(int32_t off) {
        checkAvailable(off, 1);

        return bytes[off];
    }

    bool readBool(int32_t off) {
        checkAvailable(off, 1);

        int8_t val = bytes[off];

        return val != 0;
    }

    void readByteArray(int32_t off, int32_t size, std::vector<int8_t>& res) {
        if (size == 0)
            return;

        checkAvailable(off, size);

        int32_t sizeBefore = res.size();

        res.resize(res.size() + size);

        std::copy(bytes.data() + off, bytes.data() + off + size, res.data() + sizeBefore);
    }

    int8_t* readByteArray(int32_t off, int32_t size) {
        checkAvailable(off, size);

        int8_t* res = new int8_t[size];

        std::copy(bytes.data() + off, bytes.data() + off + size, res);

        return res;
    }

    int32_t readInt32(int32_t off) {
        checkAvailable(off, 4);

        int32_t res;

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + 4;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::copy(start, end, dst);

        return res;
    }

    void readInt32Array(int32_t off, int32_t size, std::vector<int32_t>& res) {
        if (size == 0)
            return;

        checkAvailable(off, size * 4);

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 4;

        int32_t sizeBefore = res.size();

        res.resize(res.size() + size);

        int8_t* dst = reinterpret_cast<int8_t*>(res.data() + sizeBefore);

        std::copy(start, end, dst);
    }

    int32_t* readInt32Array(int32_t off, int32_t size) {
        checkAvailable(off, size * 4);

        int32_t* res = new int32_t[size];

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 4;

        int8_t* dst = reinterpret_cast<int8_t*>(res);

        std::copy(start, end, dst);

        return res;
    }

    int16_t readInt16(int32_t off) {
        checkAvailable(off, 2);

        int16_t res;

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + 2;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::copy(start, end, dst);

        return res;
    }

    void readInt16Array(int32_t off, int32_t size, std::vector<int16_t>& res) {
        if (size == 0)
            return;

        checkAvailable(off, size * 2);

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 2;

        int32_t sizeBefore = res.size();

        res.resize(res.size() + size);

        int8_t* dst = reinterpret_cast<int8_t*>(res.data() + sizeBefore);

        std::copy(start, end, dst);
    }

    int16_t* readInt16Array(int32_t off, int32_t size) {
        checkAvailable(off, size * 2);

        int16_t* res = new int16_t[size];

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 2;

        int8_t* dst = reinterpret_cast<int8_t*>(res);

        std::copy(start, end, dst);

        return res;
    }

    uint16_t readChar(int32_t off) {
        checkAvailable(off, 2);

        uint16_t res;

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + 2;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::copy(start, end, dst);

        return res;
    }

    void readCharArray(int32_t off, int32_t size, std::vector<uint16_t>& res) {
        if (size == 0)
            return;

        checkAvailable(off, size * 2);

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 2;

        int32_t sizeBefore = res.size();

        res.resize(res.size() + size);

        int8_t* dst = reinterpret_cast<int8_t*>(res.data() + sizeBefore);

        std::copy(start, end, dst);
    }

    uint16_t* readCharArray(int32_t off, int32_t size) {
        checkAvailable(off, size * 2);

        uint16_t* res = new uint16_t[size];

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 2;

        int8_t* dst = reinterpret_cast<int8_t*>(res);

        std::copy(start, end, dst);

        return res;
    }

    int64_t readInt64(int32_t off) {
        checkAvailable(off, 8);

        int64_t res;

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + 8;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::copy(start, end, dst);

        return res;
    }

    void readInt64Array(int32_t off, int32_t size, std::vector<int64_t>& res) {
        if (size == 0)
            return;

        checkAvailable(off, size * 8);

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 8;

        int32_t sizeBefore = res.size();

        res.resize(res.size() + size);

        int8_t* dst = reinterpret_cast<int8_t*>(res.data() + sizeBefore);

        std::copy(start, end, dst);
    }

    int64_t* readInt64Array(int32_t off, int32_t size) {
        checkAvailable(off, size * 8);

        int64_t* res = new int64_t[size];

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 8;

        int8_t* dst = reinterpret_cast<int8_t*>(res);

        std::copy(start, end, dst);

        return res;
    }

    float readFloat(int32_t off) {
        checkAvailable(off, 4);

        float res;

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + 4;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::copy(start, end, dst);

        return res;
    }

    void readFloatArray(int32_t off, int32_t size, std::vector<float>& res) {
        if (size == 0)
            return;

        checkAvailable(off, size * 4);

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 4;

        int32_t sizeBefore = res.size();

        res.resize(res.size() + size);

        int8_t* dst = reinterpret_cast<int8_t*>(res.data() + sizeBefore);

        std::copy(start, end, dst);
    }

    float* readFloatArray(int32_t off, int32_t size) {
        checkAvailable(off, size * 4);

        float* res = new float[size];

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 4;

        int8_t* dst = reinterpret_cast<int8_t*>(res);

        std::copy(start, end, dst);

        return res;
    }

    double readDouble(int32_t off) {
        checkAvailable(off, 8);

        double res;

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + 8;

        int8_t* dst = reinterpret_cast<int8_t*>(&res);

        std::copy(start, end, dst);

        return res;
    }

    void readDoubleArray(int32_t off, int32_t size, std::vector<double>& res) {
        if (size == 0)
            return;

        checkAvailable(off, size * 8);

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 8;

        int32_t sizeBefore = res.size();

        res.resize(res.size() + size);

        int8_t* dst = reinterpret_cast<int8_t*>(res.data() + sizeBefore);

        std::copy(start, end, dst);
    }

    double* readDoubleArray(int32_t off, int32_t size) {
        checkAvailable(off, size * 8);

        double* res = new double[size];

        const int8_t* start = bytes.data() + off;
        const int8_t* end = start + size * 8;

        int8_t* dst = reinterpret_cast<int8_t*>(res);

        std::copy(start, end, dst);

        return res;
    }

    std::vector<int8_t>& bytes;

protected:
    void checkAvailable(int32_t off, size_t cnt) {
        assert(off + cnt <= bytes.size());
    }
};
#endif

class GridPortableReaderImpl : public GridPortableReader, public GridPortableRawReader {
public:
    const int32_t start;

    int32_t rawOff;

    int32_t off;

    bool offInit;

    int32_t curTypeId;

    boost::unordered_map<int32_t, int32_t> fieldOffs;

    boost::shared_ptr<PortableReadContext> ctxPtr;

    PortableInput in;

    GridPortableReaderImpl(boost::shared_ptr<PortableReadContext> ctxPtr, int32_t start) : ctxPtr(ctxPtr),
        in(*ctxPtr.get()->dataPtr.get()), start(start), off(start), rawOff(0), curTypeId(0), offInit(false) {
    }

    GridPortable* deserializePortable() {
        boost::unordered_map<int32_t, void*>& handles = ctxPtr.get()->handles;

        boost::unordered_map<int32_t, void*>::const_iterator handle = handles.find(start);

        bool userType = in.readByte(start + 1) != 0;

        curTypeId = in.readInt32(start + 2);

        rawOff = start + in.readInt32(start + 14);

        if (handle == handles.end()) {
            GridPortable* portable = userType ? createPortable(curTypeId, *this) : createSystemPortable(curTypeId, *this);

            handles[start] = portable;

            portable->readPortable(*this);

            return portable;
        }
        else {
            GridPortable* portable = reinterpret_cast<GridPortable*>((*handle).second);

            return portable;
        }
    }

    GridClientVariant doReadVariant(bool raw) {
        return unmarshal(raw);
    }

    GridClientVariant unmarshal(bool raw) {
        int8_t flag = doReadByte(raw);

        switch(flag) {
            case FLAG_NULL: {
                return GridClientVariant();
            }

            case FLAG_HANDLE: {
                int32_t objStart = doReadInt32(raw);

                objStart = (raw ? rawOff : off) - objStart - 5;

                bool userType = in.readByte(objStart + 1) != 0;

                if (!userType) {
                    int32_t typeId = in.readInt32(objStart + 2);

                    int32_t curOff = raw ? rawOff : off;

                    if (raw)
                        rawOff = objStart + 18;
                    else
                        off = objStart + 18;

                    GridClientVariant res = readStandard(typeId, raw);

                    if (raw)
                        rawOff = curOff;
                    else
                        off = curOff;

                    return res;
                }

                GridPortableObject obj(ctxPtr, objStart);

                return GridClientVariant(obj);
            }

            case FLAG_OBJECT: {
                int32_t objStart = raw ? (rawOff - 1) : (off - 1);

                bool userType = doReadBool(raw);

                int32_t typeId = doReadInt32(raw);

                int32_t hashCode = doReadInt32(raw);

                int32_t len = doReadInt32(raw);

                int32_t rawOff = doReadInt32(raw);

                if (userType) {
                    GridPortableObject obj(ctxPtr, objStart);

                    if (raw)
                        this->rawOff = objStart + len;
                    else
                        this->off = objStart + len;

                    return GridClientVariant(obj);
                }
                else
                    return readStandard(typeId, raw);
            }

            default:
                throw GridClientPortableException("Invalid flag.");
        }
    }

    GridClientVariant unmarshalFieldStr(const std::string& fieldName) {
        off = fieldOffset(fieldName);

        return doUnmarshalField();
    }

    GridClientVariant unmarshalField(const char* fieldName) {
        off = fieldOffset(fieldName);

        return doUnmarshalField();
    }

    GridClientVariant doUnmarshalField() {
        if (off == -1)
            return GridClientVariant();

        int8_t flag = doReadByte(false);

        switch(flag) {
            case FLAG_OBJECT: {
                int32_t objStart = off - 1;

                bool userType = doReadBool(false);

                int32_t typeId = doReadInt32(false);

                int32_t hashCode = doReadInt32(false);

                int32_t len = doReadInt32(false);

                int32_t rawOff = doReadInt32(false);

                if (userType) {
                    GridPortableObject obj(ctxPtr, objStart);

                    this->off = objStart + len;

                    return GridClientVariant(obj);
                }
                else
                    return readStandard(typeId, false);
            }

            case FLAG_HANDLE: {
                int32_t objStart = doReadInt32(false);

                objStart = off - objStart - 5;

                bool userType = in.readByte(objStart + 1) != 0;

                if (!userType) {
                    int32_t typeId = in.readInt32(objStart + 2);

                    int32_t curOff = off;

                    GridClientVariant res = readStandard(typeId, false);

                    off = curOff;

                    return res;
                }

                GridPortableObject obj(ctxPtr, objStart);

                return GridClientVariant(obj);
            }

            case FLAG_NULL: {
                return GridClientVariant();
            }
        }

        return readStandard(flag, false);
    }

    GridClientVariant readStandard(int32_t typeId, bool raw) {
        if (typeId == TYPE_ID_PORTABLE) {
            int32_t arrLen = doReadInt32(raw);

            int32_t start = raw ? rawOff : off;

            if (raw)
                this->rawOff += arrLen;
            else
                this->off += arrLen;

            int32_t portableStart = start + doReadInt32(raw);

            bool userType = in.readByte(portableStart + 1) != 0;

            if (!userType) {
                int32_t typeId = in.readInt32(portableStart + 2);

                int32_t curOff = raw ? rawOff : off;

                if (raw)
                    rawOff = portableStart + 18;
                else
                    off = portableStart + 18;

                GridClientVariant res = readStandard(typeId, raw);

                if (raw)
                    rawOff = curOff;
                else
                    off = curOff;

                return res;
            }
            else {
                GridPortableObject obj(ctxPtr, portableStart);

                return GridClientVariant(obj);
            }
        }
        else if (systemPortable(typeId)) {
            int32_t start = raw ? rawOff - 18 : off - 18;

            GridPortableObject obj(ctxPtr, start);

            int32_t len = in.readInt32(start + 10);

            if (raw)
                this->rawOff = start + len;
            else
                this->off = start + len;

            return GridClientVariant(obj);
        }

        switch (typeId) {
            case TYPE_ID_BYTE: {
                int8_t val = doReadByte(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_BOOLEAN: {
                bool val = doReadBool(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_SHORT: {
                int16_t val = doReadInt16(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_CHAR: {
                uint16_t val = doReadChar(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_INT: {
                int32_t val = doReadInt32(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_LONG: {
                int64_t val = doReadInt64(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_FLOAT: {
                float val = doReadFloat(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_DOUBLE: {
                double val = doReadDouble(raw);

                return GridClientVariant(val);
            }

            case TYPE_ID_STRING: {
                boost::optional<std::string> val = doReadString(raw);

                if (val)
                    return GridClientVariant(val.get());
            }

            case TYPE_ID_UUID: {
                boost::optional<GridClientUuid> val = doReadUuid(raw);

                if (val)
                    return GridClientVariant(val.get());
            }

            case TYPE_ID_DATE: {
                boost::optional<GridClientDate> val = doReadDate(raw);

                if (val)
                    return GridClientVariant(val.get());
            }

            case TYPE_ID_BYTE_ARR: {
                std::vector<int8_t> val;

                if (doReadByteArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_BOOLEAN_ARR: {
                std::vector<bool> val;

                if (doReadBoolArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_SHORT_ARR: {
                std::vector<int16_t> val;

                if (doReadInt16Array(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_CHAR_ARR: {
                std::vector<uint16_t> val;

                if (doReadCharArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_INT_ARR: {
                std::vector<int32_t> val;

                if (doReadInt32Array(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_LONG_ARR: {
                std::vector<int64_t> val;

                if (doReadInt64Array(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_FLOAT_ARR: {
                std::vector<float> val;

                if (doReadFloatArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_DOUBLE_ARR: {
                std::vector<double> val;

                if (doReadDoubleArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_STRING_ARR: {
                std::vector<std::string> val;

                if (doReadStringArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_UUID_ARR: {
                std::vector<GridClientUuid> val;

                if (doReadUuidArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_DATE_ARR: {
                std::vector<GridClientDate> val;

                if (doReadDateArray(raw, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_OBJ_ARR: {
                std::vector<GridClientVariant> val;

                if (doReadVariantCollection(raw, false, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_COLLECTION: {
                std::vector<GridClientVariant> val;

                if (doReadVariantCollection(raw, true, val))
                    return GridClientVariant(std::move(val));
            }

            case TYPE_ID_MAP: {
                TGridClientVariantMap val;

                if (doReadVariantMap(raw, val))
                    return GridClientVariant(std::move(val));
            }

            default: {
                std::ostringstream msg;

                msg << "Invalid type " << typeId;

                throw GridClientPortableException(msg.str());
            }
        }

        return GridClientVariant();
    }

    void checkType(int8_t expFlag, int8_t flag) {
       if (flag != expFlag) {
            std::ostringstream msg;

            msg << "Invalid field type [exp=" << expFlag << ", actual=" << flag << "]";

            throw GridClientPortableException(msg.str());
       }
    }

    void checkType(int8_t expFlag1, int8_t expFlag2, int8_t flag) {
       if (flag != expFlag1 && flag != expFlag2) {
            std::ostringstream msg;

            msg << "Invalid field type [exp=" << expFlag1 << " or " << expFlag2 << ", actual=" << flag << "]";

            throw GridClientPortableException(msg.str());
       }
    }

    int32_t fieldOffset(const std::string& fieldName) {
        int32_t id = getFieldId(fieldName, curTypeId, ctxPtr.get()->idRslvr);

        return fieldOffset(id);
    }

    int32_t fieldOffset(const char* fieldName) {
        int32_t id = getFieldId(fieldName, curTypeId, ctxPtr.get()->idRslvr);

        return fieldOffset(id);
    }

    int32_t fieldOffset(int32_t id) {
        if (!offInit) {
            int32_t off = start + 18;

            int32_t rawOff = in.readInt32(start + 14);

            int32_t end = start + rawOff;

            while(true) {
                if (off >= end)
                    break;

                int32_t id = in.readInt32(off);

                off += 4;

                int32_t len = in.readInt32(off);

                off += 4;

                fieldOffs[id] = off;

                off += len;
            }

            offInit = true;
        }

        boost::unordered_map<int32_t, int32_t>::const_iterator off = fieldOffs.find(id);

        if (off == fieldOffs.end())
            return -1;

        return (*off).second;
    }

    int8_t doReadByte(bool raw) {
        if (raw)
            return in.readByte(rawOff++);
        else
            return in.readByte(off++);
    }

    bool doReadBool(bool raw) {
        int8_t val;

        if (raw)
            val = in.readByte(rawOff++);
        else
            val = in.readByte(off++);

        return val != 0;
    }

    int16_t doReadInt16(bool raw) {
        int16_t res;

        if (raw) {
            res = in.readInt16(rawOff);

            rawOff += 2;
        }
        else {
            res = in.readInt16(off);

            off += 2;
        }

        return res;
    }

    uint16_t doReadChar(bool raw) {
        uint16_t res;

        if (raw) {
            res = in.readChar(rawOff);

            rawOff += 2;
        }
        else {
            res = in.readChar(off);

            off += 2;
        }

        return res;
    }

    int32_t doReadInt32(bool raw) {
        int32_t res;

        if (raw) {
            res = in.readInt32(rawOff);

            rawOff += 4;
        }
        else {
            res = in.readInt32(off);

            off += 4;
        }

        return res;
    }

    int64_t doReadInt64(bool raw) {
        int64_t res;

        if (raw) {
            res = in.readInt64(rawOff);

            rawOff += 8;
        }
        else {
            res = in.readInt64(off);

            off += 8;
        }

        return res;
    }

    float doReadFloat(bool raw) {
        float res;

        if (raw) {
            res = in.readFloat(rawOff);

            rawOff += 4;
        }
        else {
            res = in.readFloat(off);

            off += 4;
        }

        return res;
    }

    double doReadDouble(bool raw) {
        double res;

        if (raw) {
            res = in.readDouble(rawOff);

            rawOff += 8;
        }
        else {
            res = in.readDouble(off);

            off += 8;
        }

        return res;
    }

    GridPortableRawReader& rawReader() {
        return *this;
    }

    int8_t readByte(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_BYTE, flag);

            return doReadByte(false);
        }
        else
            return 0;
    }

    bool startReadField(char* fieldName) {
        off = fieldOffset(fieldName);

        return off >= 0;
    }

    int32_t readArraySize(bool raw) {
        return doReadInt32(raw);
    }

    int8_t readByte0(bool raw) {
        return doReadByte(raw);
    }

    bool startReadBoolArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_BOOLEAN_ARR);
    }

    bool startReadByteArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_BYTE_ARR);
    }

    bool startReadInt16Array(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_SHORT_ARR);
    }

    bool startReadCharArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_CHAR_ARR);
    }

    bool startReadInt32Array(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_INT_ARR);
    }

    bool startReadInt64Array(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_LONG_ARR);
    }

    bool startReadFloatArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_FLOAT_ARR);
    }

    bool startReadDoubleArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_DOUBLE_ARR);
    }

    bool startReadUuidArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_UUID_ARR);
    }

    bool startReadDateArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_DATE_ARR);
    }

    bool startReadStringArray(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_STRING_ARR);
    }

    bool startReadVariantCollection(char* fieldName) {
        return startReadArray(fieldName, TYPE_ID_COLLECTION);
    }

    bool startReadArray(char* fieldName, int8_t type) {
        off = fieldOffset(fieldName);

        if (off < 0)
            return false;

        int8_t flag = doReadByte(false);

        checkType(type, flag);

        return off;
    }

    std::string doReadString(int32_t len, bool raw) {
        char* start = reinterpret_cast<char*>(in.bytes.data() + (raw ? rawOff : off));

        if (raw)
            rawOff += len;
        else
            off += len;

        return std::string(start, len);
    }

    std::wstring doReadWString(int32_t len, bool raw) {
        wchar_t* start = reinterpret_cast<wchar_t*>(in.bytes.data() + (raw ? rawOff : off));

        if (raw)
            rawOff += len;
        else
            off += len;

        return std::wstring(start, len / sizeof(wchar_t));
    }

    GridClientUuid doReadUuidObject(bool raw) {
        int64_t most = doReadInt64(raw);
        int64_t least = doReadInt64(raw);

        return GridClientUuid(most, least);
    }

    GridClientDate doReadDateObject(bool raw) {
        int64_t t1 = doReadInt64(raw);
        int16_t t2 = doReadInt16(raw);

        return GridClientDate(t1, t2);
    }

    bool readByteArray(char* fieldName, std::vector<int8_t>& arr) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_BYTE_ARR, flag);

            return doReadByteArray(false, arr);
        }
        else
            return false;
    }

    bool doReadByteArray(bool raw, std::vector<int8_t>& res)  {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                in.readByteArray(rawOff, len, res);

                rawOff += len;
            }
            else {
                in.readByteArray(off, len, res);

                off += len;
            }

            return true;
        }

        return false;
    }

    std::pair<int8_t*, int32_t> readByteArray(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_BYTE_ARR, flag);

            return doReadByteArray(false);
        }

        return std::pair<int8_t*, int32_t>((int8_t*)0, 0);
    }

    std::pair<int8_t*, int32_t> doReadByteArray(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                std::pair<int8_t*, int32_t> res(in.readByteArray(rawOff, len), len);

                rawOff += len;

                return res;
            }
            else {
                std::pair<int8_t*, int32_t> res(in.readByteArray(off, len), len);

                off += len;

                return res;
            }
        }
        else
            return std::pair<int8_t*, int32_t>((int8_t*)0, 0);
    }

    int16_t readInt16(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_SHORT, flag);

            return doReadInt16(false);
        }
        else
            return 0;
    }

    std::pair<int16_t*, int32_t> readInt16Array(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_SHORT_ARR, flag);

            return doReadInt16Array(false);
        }

        return std::pair<int16_t*, int32_t>((int16_t*)0, 0);
    }

    std::pair<int16_t*, int32_t> doReadInt16Array(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                std::pair<int16_t*, int32_t> res(in.readInt16Array(rawOff, len), len);

                rawOff += len * 2;

                return res;
            }
            else {
                std::pair<int16_t*, int32_t> res(in.readInt16Array(off, len), len);

                off += len * 2;

                return res;
            }
        }
        else
            return std::pair<int16_t*, int32_t>((int16_t*)0, 0);
    }

    bool readInt16Array(char* fieldName, std::vector<int16_t>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_SHORT_ARR, flag);

            return doReadInt16Array(false, res);
        }
        else
            return false;
    }

    bool doReadInt16Array(bool raw, std::vector<int16_t>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                in.readInt16Array(rawOff, len, res);

                rawOff += len * 2;
            }
            else {
                in.readInt16Array(off, len, res);

                off += len * 2;
            }

            return true;
        }

        return false;
    }

    uint16_t readChar(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_CHAR, flag);

            return doReadChar(false);
        }
        else
            return 0;
    }

    std::pair<uint16_t*, int32_t> readCharArray(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_CHAR_ARR, flag);

            return doReadCharArray(false);
        }

        return std::pair<uint16_t*, int32_t>((uint16_t*)0, 0);
    }

    std::pair<uint16_t*, int32_t> doReadCharArray(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                std::pair<uint16_t*, int32_t> res(in.readCharArray(rawOff, len), len);

                rawOff += len * 2;

                return res;
            }
            else {
                std::pair<uint16_t*, int32_t> res(in.readCharArray(off, len), len);

                off += len * 2;

                return res;
            }
        }
        else
            return std::pair<uint16_t*, int32_t>((uint16_t*)0, 0);
    }

    bool readCharArray(char* fieldName, std::vector<uint16_t>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_CHAR_ARR, flag);

            return doReadCharArray(false, res);
        }
        else
            return false;
    }

    bool doReadCharArray(bool raw, std::vector<uint16_t>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                in.readCharArray(rawOff, len, res);

                rawOff += len * 2;
            }
            else {
                in.readCharArray(off, len, res);

                off += len * 2;
            }

            return true;
        }

        return false;
    }


    int32_t readInt32(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_INT, flag);

            return doReadInt32(false);
        }
        else
            return 0;
    }

    std::pair<int32_t*, int32_t> readInt32Array(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_INT_ARR, flag);

            return doReadInt32Array(false);
        }

        return std::pair<int32_t*, int32_t>((int32_t*)0, 0);
    }

    std::pair<int32_t*, int32_t> doReadInt32Array(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                std::pair<int32_t*, int32_t> res(in.readInt32Array(rawOff, len), len);

                rawOff += len * 4;

                return res;
            }
            else {
                std::pair<int32_t*, int32_t> res(in.readInt32Array(off, len), len);

                off += len * 4;

                return res;
            }
        }
        else
            return std::pair<int32_t*, int32_t>((int32_t*)0, 0);
    }

    bool readInt32Array(char* fieldName, std::vector<int32_t>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_INT_ARR, flag);

            return doReadInt32Array(false, res);
        }
        else
            return false;
    }

    bool doReadInt32Array(bool raw, std::vector<int32_t>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                in.readInt32Array(rawOff, len, res);

                rawOff += len * 4;
            }
            else {
                in.readInt32Array(off, len, res);

                off += len * 4;
            }

            return true;
        }
        else
            return false;
    }

    int64_t readInt64(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_LONG, flag);

            return doReadInt64(false);
        }
        else
            return 0;
    }

    std::pair<int64_t*, int32_t> readInt64Array(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_LONG_ARR, flag);

            return doReadInt64Array(false);
        }

        return std::pair<int64_t*, int32_t>((int64_t*)0, 0);
    }

    std::pair<int64_t*, int32_t> doReadInt64Array(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                std::pair<int64_t*, int32_t> res(in.readInt64Array(rawOff, len), len);

                rawOff += len * 8;

                return res;
            }
            else {
                std::pair<int64_t*, int32_t> res(in.readInt64Array(off, len), len);

                off += len * 8;

                return res;
            }
        }
        else
            return std::pair<int64_t*, int32_t>((int64_t*)0, 0);
    }

    bool readInt64Array(char* fieldName, std::vector<int64_t>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_LONG_ARR, flag);

            return doReadInt64Array(false, res);
        }
        else
            return false;
    }

    bool doReadInt64Array(bool raw, std::vector<int64_t>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                in.readInt64Array(rawOff, len, res);

                rawOff += len * 8;
            }
            else {
                in.readInt64Array(off, len, res);

                off += len * 8;
            }

            return true;
        }

        return false;
    }

    float readFloat(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_FLOAT, flag);

            return doReadFloat(false);
        }
        else
            return 0;
    }

    std::pair<float*, int32_t> readFloatArray(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_FLOAT_ARR, flag);

            return doReadFloatArray(false);
        }

        return std::pair<float*, int32_t>((float*)0, 0);
    }

    std::pair<float*, int32_t> doReadFloatArray(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                std::pair<float*, int32_t> res(in.readFloatArray(rawOff, len), len);

                rawOff += len * 4;

                return res;
            }
            else {
                std::pair<float*, int32_t> res(in.readFloatArray(off, len), len);

                off += len * 4;

                return res;
            }
        }
        else
            return std::pair<float*, int32_t>((float*)0, 0);
    }

    bool readFloatArray(char* fieldName, std::vector<float>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_FLOAT_ARR, flag);

            return doReadFloatArray(false, res);
        }
        else
            return false;
    }

    bool doReadFloatArray(bool raw, std::vector<float>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                in.readFloatArray(rawOff, len, res);

                rawOff += len * 4;
            }
            else {
                in.readFloatArray(off, len, res);

                off += len * 4;
            }

            return true;
        }

        return false;
    }

    double readDouble(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_DOUBLE, flag);

            return doReadDouble(false);
        }
        else
            return 0;
    }

    std::pair<double*, int32_t> readDoubleArray(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_DOUBLE_ARR, flag);

            return doReadDoubleArray(false);
        }

        return std::pair<double*, int32_t>((double*)0, 0);
    }

    std::pair<double*, int32_t> doReadDoubleArray(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                std::pair<double*, int32_t> res(in.readDoubleArray(rawOff, len), len);

                rawOff += len * 8;

                return res;
            }
            else {
                std::pair<double*, int32_t> res(in.readDoubleArray(off, len), len);

                off += len * 8;

                return res;
            }
        }
        else
            return std::pair<double*, int32_t>((double*)0, 0);
    }

    bool readDoubleArray(char* fieldName, std::vector<double>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_DOUBLE_ARR, flag);

            return doReadDoubleArray(false, res);
        }
        else
            return false;
    }

    bool doReadDoubleArray(bool raw, std::vector<double>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            if (raw) {
                in.readDoubleArray(rawOff, len, res);

                rawOff += len * 8;
            }
            else {
                in.readDoubleArray(off, len, res);

                off += len * 8;
            }

            return true;
        }

        return false;
    }

    boost::optional<std::string> readString(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_STRING, flag);

            return doReadString(false);
        }
        else
            return 0;
    }

    boost::optional<std::string> doReadString(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            char* start = reinterpret_cast<char*>(in.bytes.data() + (raw ? rawOff : off));

            if (raw)
                rawOff += len;
            else
                off += len;

            return boost::optional<std::string>(boost::in_place(start, len));
        }
        else
            return boost::optional<std::string>();
    }

    bool readStringArray(char* fieldName, std::vector<std::string>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_STRING_ARR, flag);

            return doReadStringArray(false, res);
        }
        else
            return false;
    }

    bool doReadStringArray(bool raw, std::vector<std::string>& res)  {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            res.reserve(res.size() + len);

            for (int i = 0; i < len; i++) {
                int32_t strLen = doReadInt32(raw);

                if (strLen >= 0) {
                    char* start = reinterpret_cast<char*>(in.bytes.data() + (raw ? rawOff : off));

                    if (raw)
                        rawOff += strLen;
                    else
                        off += strLen;

                    res.push_back(std::string(start, strLen));
                }
            }

            return true;
        }
        else
            return false;
    }

    boost::optional<std::wstring> readWString(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_STRING, flag);

            return doReadWString(false);
        }
        else
            return 0;
    }

    boost::optional<std::wstring> doReadWString(bool raw) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            wchar_t* start = reinterpret_cast<wchar_t*>(in.bytes.data() + (raw ? rawOff : off));

            if (raw)
                rawOff += len;
            else
                off += len;

            return boost::optional<std::wstring>(boost::in_place(start, len / sizeof(wchar_t)));
        }
        else
            return boost::optional<std::wstring>();
    }

    bool readWStringArray(char* fieldName, std::vector<std::wstring>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_STRING_ARR, flag);

            return doReadWStringArray(false, res);
        }
        else
            return false;
    }

    bool doReadWStringArray(bool raw, std::vector<std::wstring>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            res.reserve(res.size() + len);

            for (int i = 0; i < len; i++) {
                int32_t strLen = doReadInt32(raw);

                if (strLen >= 0) {
                    wchar_t* start = reinterpret_cast<wchar_t*>(in.bytes.data() + (raw ? rawOff : off));

                    if (raw)
                        rawOff += strLen;
                    else
                        off += strLen;

                    res.emplace_back(std::wstring(start, strLen / sizeof(wchar_t)));
                }
            }

            return true;
        }
        else
            return false;
    }

    bool readBool(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_BOOLEAN, flag);

            return doReadBool(false);
        }
        else
            return 0;
    }

    bool readBoolArray(char* fieldName, std::vector<bool>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_BOOLEAN_ARR, flag);

            return doReadBoolArray(false, res);
        }
            return false;
    }

    bool doReadBoolArray(bool raw, std::vector<bool>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            res.reserve(res.size() + len);

            for (int i = 0; i < len; i++)
                res.push_back(doReadBool(raw));

            return true;
        }

        return false;
    }

    std::pair<bool*, int32_t> readBoolArray(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_BOOLEAN_ARR, flag);

            return doReadBoolArray(false);
        }

        return std::pair<bool*, int32_t>((bool*)0, 0);
    }

    std::pair<bool*, int32_t> doReadBoolArray(bool raw)  {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            bool* arr = new bool[len];

            for (int i = 0; i < len; i++)
                arr[i] = doReadBool(raw);

            return std::pair<bool*, int32_t>(arr, len);
        }
        else
            return std::pair<bool*, int32_t>((bool*)0, 0);
    }

    boost::optional<GridClientUuid> readUuid(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_UUID, flag);

            return doReadUuid(false);
        }
        else
            return 0;
    }

    boost::optional<GridClientUuid> doReadUuid(bool raw) {
        boost::optional<GridClientUuid> res;

        if (doReadBool(raw)) {
            int64_t most = doReadInt64(raw);
            int64_t least = doReadInt64(raw);

            res.reset(GridClientUuid(most, least));
        }

        return res;
    }

    boost::optional<GridClientDate> readDate(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_DATE, flag);

            return doReadDate(false);
        }
        else
            return boost::optional<GridClientDate>();
    }

    boost::optional<GridClientDate> doReadDate(bool raw) {
        if (doReadBool(raw)) {
            int64_t t1 = doReadInt64(raw);
            int16_t t2 = doReadInt16(raw);

            return boost::optional<GridClientDate>(GridClientDate(t1, t2));
        }

        return boost::optional<GridClientDate>();
    }

    bool readUuidArray(char* fieldName, std::vector<GridClientUuid>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_UUID_ARR, flag);

            return doReadUuidArray(false, res);
        }
        else
            return false;
    }

    bool doReadUuidArray(bool raw, std::vector<GridClientUuid>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            res.reserve(res.size() + len);

            for (int i = 0; i < len; i++) {
                if (doReadBool(raw))
                    res.push_back(doReadUuidObject(raw));
            }

            return true;
        }
        else
            return false;
    }

    bool readDateArray(char* fieldName, std::vector<GridClientDate>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_DATE_ARR, flag);

            return doReadDateArray(false, res);
        }
        else
            return false;
    }

    bool doReadDateArray(bool raw, std::vector<GridClientDate>& res) {
        int32_t len = doReadInt32(raw);

        if (len >= 0) {
            res.reserve(res.size() + len);

            for (int i = 0; i < len; i++) {
                if (doReadBool(raw))
                    res.push_back(doReadDateObject(raw));
            }

            return true;
        }
        else
            return false;
    }

    GridClientVariant readVariant(char* fieldName) {
        off = fieldOffset(fieldName);

        if (off >= 0)
            return unmarshal(false);

        return GridClientVariant();
    }

    bool readVariantArray(char* fieldName, std::vector<GridClientVariant>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_OBJ_ARR, flag);

            return doReadVariantCollection(false, false, res);
        }
        else
            return false;
    }

    bool readVariantCollection(char* fieldName, std::vector<GridClientVariant>& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_COLLECTION, flag);

            return doReadVariantCollection(false, true, res);
        }
        else
            return false;
    }

    bool readVariantMap(char* fieldName, TGridClientVariantMap& res) {
        off = fieldOffset(fieldName);

        if (off >= 0) {
            int8_t flag = doReadByte(false);

            checkType(TYPE_ID_MAP, flag);

            return doReadVariantMap(false, res);
        }
        else
            return false;
    }

    int8_t readByte() {
        return doReadByte(true);
    }

    bool readByteArray(std::vector<int8_t>& res) {
        return doReadByteArray(true, res);
    }

    std::pair<int8_t*, int32_t> readByteArray() {
        return doReadByteArray(true);
    }

    int16_t readInt16() {
        return doReadInt16(true);
    }

    std::pair<int16_t*, int32_t> readInt16Array() {
        return doReadInt16Array(true);
    }

    bool readInt16Array(std::vector<int16_t>& res) {
        return doReadInt16Array(true, res);
    }

    int32_t readInt32() {
        return doReadInt32(true);
    }

    std::pair<int32_t*, int32_t> readInt32Array() {
        return doReadInt32Array(true);
    }

    bool readInt32Array(std::vector<int32_t>& res) {
        return doReadInt32Array(true, res);
    }

    int64_t readInt64() {
        return doReadInt64(true);
    }

    std::pair<int64_t*, int32_t> readInt64Array() {
        return doReadInt64Array(true);
    }

    bool readInt64Array(std::vector<int64_t>& res) {
        return doReadInt64Array(true, res);
    }

    float readFloat() {
        return doReadFloat(true);
    }

    std::pair<float*, int32_t> readFloatArray() {
        return doReadFloatArray(true);
    }

    bool readFloatArray(std::vector<float>& res) {
        return doReadFloatArray(true, res);
    }

    double readDouble() {
        return doReadDouble(true);
    }

    std::pair<double*, int32_t> readDoubleArray() {
        return doReadDoubleArray(true);
    }

    bool readDoubleArray(std::vector<double>& res) {
        return doReadDoubleArray(true, res);
    }

    boost::optional<std::string> readString() {
        return doReadString(true);
    }

    bool readStringArray(std::vector<std::string>& res) {
        return doReadStringArray(true, res);
    }

    boost::optional<std::wstring> readWString() {
        return doReadWString(true);
    }

    bool readWStringArray(std::vector<std::wstring>& res) {
        return doReadWStringArray(true, res);
    }

    bool readBool() {
        return doReadBool(true);
    }

    bool readBoolArray(std::vector<bool>& res) {
        return doReadBoolArray(true, res);
    }

    std::pair<bool*, int32_t> readBoolArray() {
        return doReadBoolArray(true);
    }

    uint16_t readChar() {
        return doReadChar(true);
    }

    std::pair<uint16_t*, int32_t> readCharArray() {
        return doReadCharArray(true);
    }

    bool readCharArray(std::vector<uint16_t>& res) {
        return doReadCharArray(true, res);
    }

    boost::optional<GridClientUuid> readUuid() {
        return doReadUuid(true);
    }

    bool readUuidArray(std::vector<GridClientUuid>& res) {
        return doReadUuidArray(true, res);
    }

    boost::optional<GridClientDate> readDate() {
        return doReadDate(true);
    }

    bool readDateArray(std::vector<GridClientDate>& res) {
        return doReadDateArray(true, res);
    }

    GridClientVariant readVariant() {
        return unmarshal(true);
    }

    bool readVariantArray(TGridClientVariantSet& res) {
        return doReadVariantCollection(true, false, res);
    }

    bool readVariantCollection(TGridClientVariantSet& res) {
        return doReadVariantCollection(true, true, res);
    }

    bool readVariantMap(TGridClientVariantMap& res) {
        return doReadVariantMap(true, res);
    }

private:
    bool doReadVariantCollection(bool raw, bool readType, TGridClientVariantSet& res) {
        int32_t size = doReadInt32(raw);

        if (size == -1)
            return false;

        if (readType)
            doReadByte(raw);

        for (int i = 0; i < size; i++)
            res.push_back(std::move(unmarshal(raw)));

        return true;
    }

    bool doReadVariantMap(bool raw, TGridClientVariantMap& res) {
        int32_t size = doReadInt32(raw);

        if (size == -1)
            return false;

        int8_t type = doReadByte(raw);

        for (int i = 0; i < size; i++) {
            GridClientVariant key = unmarshal(raw);

            GridClientVariant val = unmarshal(raw);

            res.emplace(std::make_pair(std::move(key), std::move(val)));
        }

        return true;
    }
};

class GridPortableMarshaller {
public:
    GridPortableIdResolver* idRslvr;

    GridPortableMarshaller() : idRslvr(0) {
    }

    GridPortableMarshaller(GridPortableIdResolver* idRslvr) : idRslvr(idRslvr) {
    }

    boost::shared_ptr<std::vector<int8_t>> marshal(const GridClientVariant& var) {
        std::vector<int8_t>* bytes = new std::vector<int8_t>();

        WriteContext ctx(*bytes, idRslvr);

        GridPortableWriterImpl writer(ctx, 0, 0);

		writer.doWriteVariant(var);

		return boost::shared_ptr<std::vector<int8_t>>(bytes);
    }

    void createMessage(GridClientCacheRequest& msg, GridCacheRequestCommand& cacheCmd) {
        msg.key = createPortableObject(cacheCmd.getKey());

        msg.val = createPortableObject(cacheCmd.getValue());
        msg.val2 = createPortableObject(cacheCmd.getValue2());
        
        const TGridClientVariantMap& vals = cacheCmd.getValues();

        for (auto iter = vals.begin(); iter != vals.end(); ++iter) {
            const GridClientVariant key = createPortableObject(iter->first);
            const GridClientVariant val = createPortableObject(iter->second);

            msg.vals.emplace(std::make_pair(std::move(key), std::move(val)));
        }

        msg.init(cacheCmd);
    }

    GridClientVariant createPortableObject(const GridClientVariant& var) {
        if (var.hasPortableObject() || !var.hasAnyValue())
            return var;

        boost::shared_ptr<std::vector<int8_t>> data = marshal(var);

        boost::shared_ptr<PortableReadContext> ctxPtr(new PortableReadContext(data, idRslvr));

        return GridClientVariant(GridPortableObject(ctxPtr, 0));
    }

    boost::shared_ptr<std::vector<int8_t>> marshalUserObject(GridPortable& portable) {
        std::vector<int8_t>* bytes = new std::vector<int8_t>();

        WriteContext ctx(*bytes, idRslvr);

        GridPortableWriterImpl writer(ctx, portable.typeId(), 0);

		writer.writePortable(portable);

		return boost::shared_ptr<std::vector<int8_t>>(bytes);
	}

    boost::shared_ptr<std::vector<int8_t>> marshalSystemObject(GridPortable& portable) {
        std::vector<int8_t>* bytes = new std::vector<int8_t>();

        WriteContext ctx(*bytes, idRslvr);

        GridPortableWriterImpl writer(ctx, portable.typeId(), 0);

		writer.writeSystem(portable);

		return boost::shared_ptr<std::vector<int8_t>>(bytes);
    }

    GridClientVariant unmarshal(const boost::shared_ptr<std::vector<int8_t>>& data) {
		boost::shared_ptr<PortableReadContext> ctxPtr(new PortableReadContext(data, idRslvr));

        GridPortableReaderImpl reader(ctxPtr, 0);

        return reader.unmarshal(false);
    }

    template<typename T>
    T* unmarshalUserObject(const boost::shared_ptr<std::vector<int8_t>>& data) {
        GridClientVariant var = unmarshal(data);

        GridPortableObject& portable = var.getPortableObject();

        return portable.deserialize<T>();
    }

    void parseResponse(GridClientResponse* msg, GridClientMessageTopologyResult& resp) {
        GridClientVariant res = msg->getResult();

        std::vector<GridClientNode> nodes;

        if (res.hasVariantVector()) {
            std::vector<GridClientVariant>& vec = res.getVariantVector();

            for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
                GridClientVariant var = *iter;

                std::unique_ptr<GridClientNodeBean> nodeBean(var.getPortableObject().deserialize<GridClientNodeBean>());

                nodes.push_back(nodeBean->createNode());
            }
        }
        else if (res.hasPortableObject()) {
            std::unique_ptr<GridClientNodeBean> nodeBean(res.getPortableObject().deserialize<GridClientNodeBean>());

            nodes.push_back(nodeBean->createNode());
        }

        resp.setNodes(nodes);
    }

    void parseResponse(GridClientResponse* msg, GridClientMessageLogResult& resp) {
        GridClientVariant res = msg->getResult();

        assert(res.hasVariantVector());

        std::vector<GridClientVariant> vec = res.getVariantVector();

        std::vector<std::string> lines;

        for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
            GridClientVariant line = *iter;

            assert(line.hasString());

            lines.push_back(line.getString());
        }

        resp.lines(lines);
    }

    void parseResponse(GridClientResponse* msg, GridClientMessageCacheGetResult& resp) {
        GridClientVariant res = msg->getResult();

        if (res.hasVariantMap()) {
            TCacheValuesMap keyValues = res.getVariantMap();

            resp.setCacheValues(keyValues);
        }
        else {
            TCacheValuesMap keyValues;

            keyValues.insert(std::make_pair(GridClientVariant(), res));

            resp.setCacheValues(keyValues);
        }
    }

    void parseResponse(GridClientResponse* msg, GridClientMessageCacheModifyResult& resp) {
        GridClientVariant res = msg->getResult();

        if (res.hasBool())
            resp.setOperationResult(res.getBool());
        else
            resp.setOperationResult(false);
    }

    void parseResponse(GridClientResponse* msg, GridClientMessageCacheMetricResult& resp) {
        GridClientVariant res = msg->getResult();

        assert(res.hasVariantMap());

        resp.setCacheMetrics(res.getVariantMap());
    }

    void parseResponse(GridClientResponse* msg, GridClientMessageTaskResult& resp) {
        GridClientVariant res = msg->getResult();

        if (res.hasPortableObject()) {
            std::unique_ptr<GridClientTaskResultBean> resBean(res.getPortableObject().deserialize<GridClientTaskResultBean>());

            resp.setTaskResult(resBean->res);
        }
    }

};

#endif
