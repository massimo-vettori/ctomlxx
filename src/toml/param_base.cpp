#include <ctomlxx.h>

using namespace toml;

Param::Param(const Param& clone) {
   if (clone.content) {
      switch (clone.content->type_id()) {
         case BOOL:   content = new Bool(clone.content->to_bool());     break;
         case INT:    content = new Int(clone.content->to_int());       break;
         case FLOAT:  content = new Float(clone.content->to_float());   break;
         case STRING: content = new String(clone.content->to_string()); break;

         default: content = new Generic;
      }

      allocated = true;
      name = clone.name;
   }
}

Param::~Param() {
   if (allocated) {
      delete content;
   }
}

bool Param::is_bool()   const { return allocated and content->is_bool();   }
bool Param::is_int()    const { return allocated and content->is_int();    }
bool Param::is_float()  const { return allocated and content->is_float();  }
bool Param::is_string() const { return allocated and content->is_string(); }

types Param::type_id()          const { return allocated ? content->type_id() : types::NIL; }
const string& Param::get_name() const { return this->name;                                  }

bool Param::to_bool() const {
   if (not allocated) throw nil_param_content{"The content of this toml::Param instance is null!"};
   return this->content->to_bool();
}

int Param::to_int() const {
   if (not allocated) throw nil_param_content{"The content of this toml::Param instance is null!"};
   return this->content->to_int();
}

float Param::to_float() const {
   if (not allocated) throw nil_param_content{"The content of this toml::Param instance is null!"};
   return this->content->to_float();
}

const string& Param::to_string() const {
   if (not allocated) throw nil_param_content{"The content of this toml::Param instance is null!"};
   return this->content->to_string();
}
