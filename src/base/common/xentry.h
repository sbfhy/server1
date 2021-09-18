#pragma once 

#include "noncopyable.h"
#include "define/define_variable.h"
#include "define/define_types.h"

const DWORD MAX_NAMESIZE = 64;

struct xEntryC
{
  public:
    xEntryC()
    {
      id = 0;
      BZERO(name, sizeof(name));
      guid = 0;
      BZERO(spawnerid, sizeof(spawnerid));
    }
    virtual ~xEntryC() {}

  public:
    // set接口
    void set_id(QWORD i) { id = i; }
    void set_cfgid(QWORD i) {cfgid = i;}
    void set_name(const char *n)
    {
      if (NULL == n) return;
      BZERO(name, sizeof(name));
      strncpy(name, n, MAX_NAMESIZE-1);
    }
    void set_guid(QWORD t) { guid = t; }
    void set_spawnerid(const char* id_) 
    {
      if (id_ == nullptr) return;
      BZERO(spawnerid, sizeof(spawnerid));
      strncpy(spawnerid, id_, MAX_NAMESIZE-1);
    }

    // get接口
    QWORD getGuid() const { return guid; }
    QWORD get_cfgid() const {return cfgid;}
    const char* get_name() const {return name;}
    const char* get_spawnerid() const {return spawnerid;}

  public:
    QWORD id = 0;
    QWORD cfgid = 0;
    char name[MAX_NAMESIZE];
    QWORD guid;
    char spawnerid[MAX_NAMESIZE];
};

struct xEntry : public xEntryC, private muduo::noncopyable
{
  xEntry()
  {
  }
  virtual ~xEntry()
  {
  }
};
