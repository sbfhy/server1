/**
 * @file TVarList.h
 * @brief 参数集合
 */

#pragma once

#include "define/define_variable.h"
#include "define/define_new.h"

#include <string>
#include <string.h>

//class IVarTable;

class IVarList
{
public:
    virtual ~IVarList() = 0;

    // 合并
    virtual bool concat(const IVarList& src) = 0;
    // 部分添加
    virtual bool append(const IVarList& src, size_t start, size_t count) = 0;

    // 清空
    virtual void clear() = 0;
    // 是否为空
    virtual bool isEmpty() const = 0;
    // 数据数量
    virtual size_t getCount() const = 0;
    // 数据类型
    virtual int getType(size_t index) const = 0;

    // 添加数据
    virtual bool addBool(bool value) = 0;
    virtual bool addDWORD(DWORD value) = 0;
    virtual bool addQWORD(QWORD value) = 0;
    virtual bool addFloat(float value) = 0;
    virtual bool addDouble(double value) = 0;
    virtual bool addString(const char* value) = 0;
    virtual bool addPointer(void* value) = 0;

    // 获得数据
    virtual bool boolVal(size_t index) const = 0;
    virtual DWORD dwordVal(size_t index) const = 0;
    virtual QWORD qwordVal(size_t index) const = 0;
    virtual float floatVal(size_t index) const = 0;
    virtual double doubleVal(size_t index) const = 0;
    virtual const char* stringVal(size_t index) const = 0;
    virtual void* pointerVal(size_t index) const = 0;

    // 获得内存占用
    virtual size_t getMemoryUsage() const = 0;

};

inline IVarList::~IVarList() {}


#define ALING_VARLIST 4
#define ROUND_UP (((bytes) + (size_t)ALING_VARLIST - 1) & ~((size_t)ALING_VARLIST - 1))

// 参数集

class TVarListAlloc
{
public:
    TVarListAlloc() {}
    ~TVarListAlloc() {}

    // 申请内存
    void* alloc(size_t size) { return NEW char[size]; }
    // 释放内存
    void free(void* ptr, size_t size)
    {
        char* pDel = static_cast<char*>(ptr);
        DELETE_VEC(pDel);
    }
    // 交换
    void swap(TVarListAlloc& src) {}
};

template<size_t DATA_SIZE, size_t BUFFER_SIZE, typename ALLOC = TVarListAlloc>
class TVarList : public IVarList
{
private:
    typedef TVarList<DATA_SIZE, BUFFER_SIZE, ALLOC> self_type;

    struct var_data_t
    {
        int nType;
        union
        {
            bool boolValue;
            DWORD intValue;
            QWORD int64Value;
            float floatValue;
            double doubleValue;
            size_t stringValue;
            void* pointerValue;
        };
    };

public:
    TVarList()
    {
        //Assert(DATA_SIZE > 0);
        //Assert(BUFFER_SIZE > 0);

        m_pData = m_DataStack;
        m_nDataSize = DATA_SIZE;
        m_nDataUsed = 0;
        m_pBuffer = m_BufferStack;
        m_nBufferSize = BUFFER_SIZE;
        m_nBufferUsed = 0;
    }

    TVarList(const self_type& src)
    {
        //Assert(DATA_SIZE > 0);
        //Assert(BUFFER_SIZE > 0);

        m_pData = m_DataStack;
        m_nDataSize = DATA_SIZE;
        m_nDataUsed = 0;
        m_pBuffer = m_BufferStack;
        m_nBufferSize = BUFFER_SIZE;
        m_nBufferUsed = 0;
        innerAppend(src, 0, src.getCount());
    }

    virtual ~TVarList()
    {
        if (m_nDataSize > DATA_SIZE)
        {
            m_Alloc.free(m_pData, m_nDataSize * sizeof(var_data_t));
        }

        if (m_nBufferSize > BUFFER_SIZE)
        {
            m_Alloc.free(m_pBuffer, m_nBufferSize);
        }
    }

    self_type& operator=(const self_type& src)
    {
        if (m_nDataSize > DATA_SIZE)
        {
            m_Alloc.free(m_pData, m_nDataSize * sizeof(var_data_t));
        }

        if (m_nBufferSize > BUFFER_SIZE)
        {
            m_Alloc.free(m_pBuffer, m_nBufferSize);
        }

        m_pData = m_DataStack;
        m_nDataSize = DATA_SIZE;
        m_nDataUsed = 0;
        m_pBuffer = m_BufferStack;
        m_nBufferSize = BUFFER_SIZE;
        m_nBufferUsed = 0;
        innerAppend(src, 0, src.getCount());

        return *this;
    }

    // 合并
    virtual bool concat(const IVarList& src)
    {
        innerAppend(src, 0, src.getCount());

        return true;
    }

    // 添加
    virtual bool append(const IVarList& src, size_t start, size_t count)
    {
        if (start >= src.getCount())
        {
            return false;
        }

        size_t end = start + count;

        if (end > src.getCount())
        {
            return false;
        }

        innerAppend(src, start, end);

        return true;
    }

    // 清空
    virtual void clear()
    {
        m_nDataUsed = 0;
        m_nBufferUsed = 0;
    }

    // 是否为空
    virtual bool isEmpty() const
    {
        return (0 == m_nDataUsed);
    }

    // 数据数量
    virtual size_t getCount() const
    {
        return m_nDataUsed;
    }

    // 数据类型
    virtual int getType(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return 0;
        }

        return m_pData[index].nType;
    }

    // 添加数据
    virtual bool addBool(bool value)
    {
        var_data_t* p = addVarData();

        p->nType = VTYPE_BOOL;
        p->boolValue = value;

        return true;
    }

    virtual bool addDWORD(DWORD value)
    {
        var_data_t* p = addVarData();

        p->nType = VTYPE_DWORD;
        p->intValue = value;

        return true;
    }

    virtual bool addQWORD(QWORD value)
    {
        var_data_t* p = addVarData();

        p->nType = VTYPE_QWORD;
        p->int64Value = value;

        return true;
    }

    virtual bool addFloat(float value)
    {
        var_data_t* p = addVarData();

        p->nType = VTYPE_FLOAT;
        p->floatValue = value;

        return true;
    }

    virtual bool addDouble(double value)
    {
        var_data_t* p = addVarData();

        p->nType = VTYPE_DOUBLE;
        p->doubleValue = value;

        return true;
    }

    virtual bool addString(const char* value)
    {
        if (nullptr == value)
            return false;

        var_data_t* p = addVarData();

        p->nType = VTYPE_STRING;
        p->stringValue = m_nBufferUsed;

        const size_t value_size = strlen(value) + 1;
        char* v = addBuffer(value_size);

        memcpy(v, value, value_size);

        return true;
    }


    virtual bool addPointer(void* value)
    {
        var_data_t* p = addVarData();

        p->nType = VTYPE_POINTER;
        p->pointerValue = value;

        return true;
    }


    // 获得数据
    virtual bool boolVal(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return false;
        }

        switch (m_pData[index].nType)
        {
        case VTYPE_BOOL:
            return m_pData[index].boolValue;
        default:
            break;
        }

        return false;
    }

    virtual DWORD dwordVal(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return 0;
        }

        switch (m_pData[index].nType)
        {
        case VTYPE_BOOL:
            return int(m_pData[index].boolValue);
        case VTYPE_DWORD:
            return m_pData[index].intValue;
        case VTYPE_QWORD:
            return int(m_pData[index].int64Value);
        case VTYPE_FLOAT:
            return int(m_pData[index].floatValue);
        case VTYPE_DOUBLE:
            return int(m_pData[index].doubleValue);
        case VTYPE_STRING:
            return atoi(m_pBuffer + m_pData[index].stringValue);
        default:
            break;
        }

        return 0;
    }

    virtual QWORD qwordVal(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return 0;
        }

        switch (m_pData[index].nType)
        {
        case VTYPE_DWORD:
            return QWORD(m_pData[index].intValue);
        case VTYPE_QWORD:
            return m_pData[index].int64Value;
        default:
            break;
        }

        return 0;
    }

    virtual float floatVal(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return 0.0f;
        }

        switch (m_pData[index].nType)
        {
        case VTYPE_FLOAT:
            return m_pData[index].floatValue;
        case VTYPE_DOUBLE:
            return float(m_pData[index].doubleValue);
        default:
            break;
        }

        return 0.0f;
    }

    virtual double doubleVal(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return 0.0;
        }

        switch (m_pData[index].nType)
        {
        case VTYPE_FLOAT:
            return double(m_pData[index].floatValue);
        case VTYPE_DOUBLE:
            return m_pData[index].doubleValue;
        default:
            break;
        }
        return 0.0;
    }

    virtual const char* stringVal(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return "";
        }

        switch (m_pData[index].nType)
        {
        case VTYPE_STRING:
            return m_pBuffer + m_pData[index].stringValue;
        default:
            break;
        }

        return "";
    }


    virtual void* pointerVal(size_t index) const
    {
        if (index >= m_nDataUsed)
        {
            return NULL;
        }

        if (m_pData[index].nType != VTYPE_POINTER)
        {
            return NULL;
        }

        return m_pData[index].pointerValue;
    }


    //设置数据
    virtual bool setDWORD(size_t index, DWORD value)
    {
        if (index >= m_nDataUsed)
            return false;
        if (m_pData[index].nType != VTYPE_DWORD)
            return false;

        m_pData[index].intValue = value;
        return true;
    }

    virtual bool setQWORD(size_t index, QWORD value)
    {
        if (index >= m_nDataUsed)
            return false;
        if (m_pData[index].nType != VTYPE_QWORD)
            return false;

        m_pData[index].int64Value = value;
        return true;
    }

    virtual bool setFloat(size_t index, float value)
    {
        if (index >= m_nDataUsed)
            return false;
        if (m_pData[index].nType != VTYPE_FLOAT)
            return false;

        m_pData[index].floatValue = value;
        return true;
    }

    virtual bool setDouble(size_t index, double value)
    {
        if (index >= m_nDataUsed)
            return false;
        if (m_pData[index].nType != VTYPE_DOUBLE)
            return false;

        m_pData[index].doubleValue = value;
        return true;
    }

    virtual bool setString(size_t index, const char* value)
    {
        if (nullptr == value)
            return false;

        if (index >= m_nDataUsed)
            return false;
        if (m_pData[index].nType != VTYPE_STRING)
            return false;

        char* ps = m_pBuffer + m_pData[index].stringValue;
        const size_t SIZE1 = strlen(value) + 1;

        //小于等于以前的,放回原地
        if (SIZE1 <= strlen(ps) + 1)
        {
            strcpy(ps, value);
            return true;
        }

        m_pData[index].stringValue = m_nBufferUsed;

        const size_t value_size = strlen(value) + 1;
        char* v = addBuffer(value_size);

        memcpy(v, value, value_size);

        return true;
    }

    virtual bool setPointer(size_t index, void* value)
    {
        if (index >= m_nDataUsed)
            return false;
        if (m_pData[index].nType != VTYPE_POINTER)
            return false;

        m_pData[index].pointerValue = value;
        return true;
    }

    // 获得内存占用
    virtual size_t getMemoryUsage() const
    {
        size_t size = sizeof(self_type);

        if (m_nDataSize > DATA_SIZE)
        {
            size += sizeof(var_data_t) * m_nDataSize;
        }

        if (m_nBufferSize > BUFFER_SIZE)
        {
            size += m_nBufferSize;
        }

        return size;
    }

    inline TVarList& operator<<(bool value)
    {
        addBool(value);
        return *this;
    }
    inline TVarList& operator<<(DWORD value)
    {
        addDWORD(value);
        return *this;
    }
    inline TVarList& operator<<(QWORD value)
    {
        addQWORD(value);
        return *this;
    }
    inline TVarList& operator<<(float value)
    {
        addFloat(value);
        return *this;
    }
    inline TVarList& operator<<(double value)
    {
        addDouble(value);
        return *this;
    }
    inline TVarList& operator<<(const char* value)
    {
        addString(value);
        return *this;
    }
    inline TVarList& operator<<(const std::string& value)
    {
        addString(value.c_str());
        return *this;
    }

    inline TVarList& operator<<(const TVarList& value)
    {
        concat(value);
        return *this;
    }

private:
    var_data_t* addVarData()
    {
        if (m_nDataUsed >= m_nDataSize)
        {
            size_t new_size = m_nDataSize * 2;
            var_data_t* p = static_cast<var_data_t*>(m_Alloc.alloc(
                new_size * sizeof(var_data_t)));

            memcpy(p, m_pData, m_nDataUsed * sizeof(var_data_t));

            if (m_nDataSize > DATA_SIZE)
            {
                m_Alloc.free(m_pData, m_nDataSize * sizeof(var_data_t));
            }

            m_pData = p;
            m_nDataSize = new_size;
        }

        return m_pData + m_nDataUsed++;
    }

    char* addBuffer(size_t need_size)
    {
        size_t new_used = m_nBufferUsed + need_size;

        if (new_used > m_nBufferSize)
        {
            size_t new_size = m_nBufferSize * 2;

            if (new_used > new_size)
            {
                new_size = new_used * 2;
            }

            char* p = static_cast<char*>(m_Alloc.alloc(new_size));

            memcpy(p, m_pBuffer, m_nBufferUsed);

            if (m_nBufferSize > BUFFER_SIZE)
            {
                m_Alloc.free(m_pBuffer, m_nBufferSize);
            }

            m_pBuffer = p;
            m_nBufferSize = new_size;
        }

        char* ret = m_pBuffer + m_nBufferUsed;

        m_nBufferUsed = new_used;

        return ret;
    }

    void innerAppend(const IVarList& src, size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            switch (src.getType(i))
            {
            case VTYPE_BOOL:
                addBool(src.boolVal(i));
                break;
            case VTYPE_DWORD:
                addDWORD(src.dwordVal(i));
                break;
            case VTYPE_QWORD:
                addQWORD(src.qwordVal(i));
                break;
            case VTYPE_FLOAT:
                addFloat(src.floatVal(i));
                break;
            case VTYPE_DOUBLE:
                addDouble(src.doubleVal(i));
                break;
            case VTYPE_STRING:
                addString(src.stringVal(i));
                break;
            case VTYPE_POINTER:
                addPointer(src.pointerVal(i));
                break;
            default:
                break;
            }
        }
    }

private:
    ALLOC m_Alloc;
    var_data_t m_DataStack[DATA_SIZE];
    var_data_t* m_pData;
    size_t m_nDataSize;
    size_t m_nDataUsed;
    char m_BufferStack[BUFFER_SIZE];
    char* m_pBuffer;
    size_t m_nBufferSize;
    size_t m_nBufferUsed;
};

typedef TVarList<8, 128> CVarList;
