#pragma once

#include <string>

using std::string;

namespace toml {

   enum types { NIL, BOOL, INT, FLOAT, STRING };

   struct name_error        { string name, reason; };
   struct invalid_type_cast { string reason;       };
   struct nil_param_content { string reason;       };

   class Generic {
      public:
         virtual ~Generic() { /* Nothing to delete yet... */ }

         virtual bool is_bool()   const { return false; }
         virtual bool is_int()    const { return false; }
         virtual bool is_float()  const { return false; }
         virtual bool is_string() const { return false; }

         virtual bool          to_bool()   const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::Bool"};   }
         virtual int           to_int()    const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::Int"};    }
         virtual float         to_float()  const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::Float"};  }
         virtual const string& to_string() const { throw invalid_type_cast{"Cannot cast toml::Generic instance to toml::String"}; }
 
         virtual types type_id() const { return types::NIL; }
   };

   class Bool : public Generic {
      public:
         explicit Bool(bool value) { this->content = value; }

         bool is_bool() const { return true;    }
         bool to_bool() const { return content; }

         int           to_int()      const { throw invalid_type_cast{"Cannot cast toml::Bool instance to toml::Int"};    }
         float         to_float()    const { throw invalid_type_cast{"Cannot cast toml::Bool instance to toml::Float"};  }
         const string& to_string()   const { throw invalid_type_cast{"Cannot cast toml::Bool instance to toml::String"}; }

         types type_id() const { return types::BOOL; }

      private:
         bool content;
   };

   class Int : public Generic {
      public:
         explicit Int(int value) { this->content = value; }

         bool is_int() const { return true;    }
         int  to_int() const { return content; }

         bool          to_bool()   const { throw invalid_type_cast{"Cannot cast toml::Int instance to toml::Bool"};   }
         float         to_float()  const { throw invalid_type_cast{"Cannot cast toml::Int instance to toml::Float"};  }
         const string& to_string() const { throw invalid_type_cast{"Cannot cast toml::Int instance to toml::String"}; }

         types type_id() const { return types::INT; }

      private:
         int content;
   };

   class Float : public Generic {
      public:
         explicit Float(float value) { this->content = value; }

         bool  is_float() const { return true;    }
         float to_float() const { return content; }

         bool          to_bool()   const { throw invalid_type_cast{"Cannot cast toml::Float instance to toml::Bool"};   }
         int           to_int()    const { throw invalid_type_cast{"Cannot cast toml::Float instance to toml::Int"};    }
         const string& to_string() const { throw invalid_type_cast{"Cannot cast toml::Float instance to toml::String"}; }

         types type_id() const { return types::FLOAT; }

      private:
         float content;
   };

   class String : public Generic {
      public:
         explicit String(const string& value) { this->content = value; }

         bool          is_string() const { return true;    }
         const string& to_string() const { return content; }
         
         bool   to_bool()   const { throw invalid_type_cast{"Cannot cast toml::String instance to toml::Bool"};  }
         int    to_int()    const { throw invalid_type_cast{"Cannot cast toml::String instance to toml::Int"};   }
         float  to_float()  const { throw invalid_type_cast{"Cannot cast toml::String instance to toml::Float"}; }

         types type_id() const { return types::STRING; }
      
      private:
         string content;
   };

   class Param {
      public:
         Param(const string& raw);         
         Param(Param& clone);
         ~Param();

         bool          is_bool()   const;
         bool          is_int()    const;
         bool          is_float()  const;
         bool          is_string() const;

         bool          to_bool()   const;
         int           to_int()    const;
         float         to_float()  const;
         const string& to_string() const;

         types         type_id()   const;
         const string& get_name()  const;

      private:
         string   name      {""};
         bool     allocated {false};
         Generic* content   {nullptr};
   };

}
