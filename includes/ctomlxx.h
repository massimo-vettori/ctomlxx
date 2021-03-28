#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace toml {

   enum types { NIL, BOOL, INT, FLOAT, STRING, VECTOR };

   struct name_error        { string name, reason; };
   struct invalid_type_cast { string reason;       };
   struct nil_param_content { string reason;       };
   struct search_error      { string reason;       };
   struct uncaught_error    { string reason;       };
   struct load_error        { string reason;       };

   /* Pre-define the classes, to avoid circular dependencies issues */
   class Generic;
   class Bool;
   class Int;
   class Float;
   class String;
   class Vector;

   class Param;
   class Section;
   class Root;

   class Generic {
      public:
         virtual ~Generic() { /* Nothing to delete yet... */ }

         virtual bool is_bool()   const { return false; }
         virtual bool is_int()    const { return false; }
         virtual bool is_float()  const { return false; }
         virtual bool is_string() const { return false; }
         virtual bool is_vector() const { return false; }

         virtual bool                 to_bool()   const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::Bool"};   }
         virtual int                  to_int()    const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::Int"};    }
         virtual float                to_float()  const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::Float"};  }
         virtual const string        &to_string() const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::String"}; }
         virtual const vector<Param> &to_vector() const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::Vector"}; }

         virtual types type_id() const { return types::NIL; }
   };

   class Bool : public Generic {
      public:
         Bool(bool value) { this->content = value; }

         bool is_bool() const { return true;    }
         bool to_bool() const { return content; }

         int                          to_int()    const { throw invalid_type_cast{"Cannot cast toml::Bool instance to toml::Int"};    }
         float                        to_float()  const { throw invalid_type_cast{"Cannot cast toml::Bool instance to toml::Float"};  }
         const string                &to_string() const { throw invalid_type_cast{"Cannot cast toml::Bool instance to toml::String"}; }
         virtual const vector<Param> &to_vector() const { throw invalid_type_cast{"Cannot cast toml::Bool instance to toml::Vector"}; }


         types type_id() const { return types::BOOL; }

      private:
         bool content;
   };

   class Int : public Generic {
      public:
         Int(int value) { this->content = value; }

         bool is_int() const { return true;    }
         int  to_int() const { return content; }

         bool                         to_bool()   const { throw invalid_type_cast{"Cannot cast toml::Int instance to toml::Bool"};   }
         float                        to_float()  const { throw invalid_type_cast{"Cannot cast toml::Int instance to toml::Float"};  }
         const string                &to_string() const { throw invalid_type_cast{"Cannot cast toml::Int instance to toml::String"}; }
         virtual const vector<Param> &to_vector() const { throw invalid_type_cast{"Cannot cast toml::Int instance to toml::Vector"}; }


         types type_id() const { return types::INT; }

      private:
         int content;
   };

   class Float : public Generic {
      public:
         Float(float value) { this->content = value; }

         bool  is_float() const { return true;    }
         float to_float() const { return content; }

         bool                         to_bool()   const { throw invalid_type_cast{"Cannot cast toml::Float instance to toml::Bool"};   }
         int                          to_int()    const { throw invalid_type_cast{"Cannot cast toml::Float instance to toml::Int"};    }
         const string                &to_string() const { throw invalid_type_cast{"Cannot cast toml::Float instance to toml::String"}; }
         virtual const vector<Param> &to_vector() const { throw invalid_type_cast{"Cannot cast toml::Float instance to toml::Vector"}; }


         types type_id() const { return types::FLOAT; }

      private:
         float content;
   };

   class String : public Generic {
      public:
         String(const string& value) { this->content = value; }

         bool          is_string() const { return true;    }
         const string &to_string() const { return content; }
         
         bool                         to_bool()   const { throw invalid_type_cast{"Cannot cast toml::String instance to toml::Bool"};  }
         int                          to_int()    const { throw invalid_type_cast{"Cannot cast toml::String instance to toml::Int"};   }
         float                        to_float()  const { throw invalid_type_cast{"Cannot cast toml::String instance to toml::Float"}; }
         virtual const vector<Param> &to_vector() const { throw invalid_type_cast{"Cannot cast toml::String instance to toml::Vector"}; }


         types type_id() const { return types::STRING; }
      
      private:
         string content;
   };

   class Vector : public Generic {
      public:
         Vector(const vector<Param>& values);
         
         bool                 is_vector() const;
         const vector<Param> &to_vector() const;

         bool          to_bool()   const { throw invalid_type_cast{"Cannot cast toml::Vector instance to toml::Bool"};   }
         int           to_int()    const { throw invalid_type_cast{"Cannot cast toml::Vector instance to toml::Int"};    }
         float         to_float()  const { throw invalid_type_cast{"Cannot cast toml::Vector instance to toml::Float"};  }
         const string &to_string() const { throw invalid_type_cast{"Cannot cast toml::Vector instance to toml::String"}; }
         
         const Param& operator [](size_t pos) const;
         const Param& at(size_t pos)          const;

         types type_id() const { return types::VECTOR; }

      private:
         vector<Param> content;
   };

   class Param {
      public:
         Param(const string& raw);         
         Param(const Param& clone);
         ~Param();

         bool is_bool()   const;
         bool is_int()    const;
         bool is_float()  const;
         bool is_string() const;
         bool is_vector() const;

         bool                 to_bool()   const;
         int                  to_int()    const;
         float                to_float()  const;
         const string        &to_string() const;
         const vector<Param> &to_vector() const;

         types         type_id()  const;
         const string &get_name() const;

      private:
         string   name      {""};
         bool     allocated {false};
         Generic* content   {nullptr};
   };

   class Section {
      public:
         Section(const string& raw);

         bool          has(const string& n)           const;
         const Param&  get(const string& n)           const;
         const Param&  at(size_t pos)                 const;
         const string& get_name()                     const;
         const Param&  operator[](const string& name) const;
         const Param&  operator[](size_t pos)         const;
         void          append(const string& raw);


         bool   empty()   const;
         size_t size()    const;
         bool undefined() const;
      
      private:
         string        name;
         vector<Param> params;
   };

   class Root {
      public:
         void append(const Section& s);

         bool           has(const string& name)        const;
         const Section& get(const string& name)        const;
         const Section& at(size_t pos)                 const;
         const Section& operator[](const string& name) const;
         const Section& operator[](size_t pos)         const;

         bool empty()  const;
         size_t size() const;
      
      private:
         vector<Section> sections;
   };

   class Parser {
      public:
         void load(const string& path);
         void parse(Root& file) const;
         void flush();

      private:
         vector<string> lines;
   };
}
