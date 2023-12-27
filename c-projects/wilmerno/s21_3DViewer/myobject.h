#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "ObjectParameters.h"
#include "mainwindow.h"

extern "C" {
#include "../backend/3d_viewer.h"
}

class MyObject {
 public:
  MyObject();
  ~MyObject();

  QString path;

  void Parsing();
  void UpdateObject(ObjectParameters* params);
  const object_t* GetPrintable();
  void getVerticesAndFaces(uint64_t* vertices, uint64_t* faces) const;
  void clearContents();

 private:
  object_t normalized_state = {0};
  object_t print_state = {0};

  void freeMemory(object_t* obj);
};

#endif  // MYOBJECT_H
