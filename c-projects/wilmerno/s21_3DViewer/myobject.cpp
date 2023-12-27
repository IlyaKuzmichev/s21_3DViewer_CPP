#include "myobject.h"

MyObject::MyObject() {}

MyObject::~MyObject() { clearContents(); }

void MyObject::Parsing() {
  auto str = path.toStdString();
  parse_obj_file(str.c_str(), &print_state);
  normalize_object(print_state, &normalized_state);

  for (uint64_t i = 0; i < print_state.v_count; ++i) {
    print_state.v_array[i] = normalized_state.v_array[i];
  }
}

void MyObject::UpdateObject(ObjectParameters* params) {
  for (uint64_t i = 0; i < print_state.v_count; ++i) {
    print_state.v_array[i] = normalized_state.v_array[i];
    rotate_ox_point(print_state.v_array + i, params->rotate_x);
    rotate_oy_point(print_state.v_array + i, params->rotate_y);
    rotate_oz_point(print_state.v_array + i, params->rotate_z);
    scale_point(print_state.v_array + i, params->scale);

    translate_point(print_state.v_array + i, X_AXIS, params->translate_x);
    translate_point(print_state.v_array + i, Y_AXIS, params->translate_y);
    translate_point(print_state.v_array + i, Z_AXIS, params->translate_z * 4.);
  }
}

const object_t* MyObject::GetPrintable() { return &print_state; }

void MyObject::freeMemory(object_t* obj) {
  if (obj->f_count > 0) {
    for (size_t i = 0; i < obj->f_count; ++i) {
      free(obj->f_array[i].v_array);
    }
    free(obj->f_array);
  }
  obj->f_array = NULL;
  free(obj->v_array);
  obj->v_array = NULL;
}

void MyObject::clearContents() {
  freeMemory(&print_state);
  normalized_state.f_count = 0;
  freeMemory(&normalized_state);
}

void MyObject::getVerticesAndFaces(uint64_t* vertices, uint64_t* faces) const {
  *vertices = print_state.v_count;
  *faces = print_state.f_count;
}
