#ifndef VAR_H
#define VAR_H

#include "eeDefines.h"

class eeVar
{
    private:
        const string _name;
        string _value;
        string _type;
        int _used;

    public:
        eeVar(const string &name, string value = "void",const string type = "void",int used = 0):
            _name(name), _value(value), _type(type), _used(used)
        {}
        ~eeVar() {}

        string getName() const
        { return _name; }

        string getValue() const
        { return _value; }

        void setValue(string value)
        { _value = value; }
     
        void setType(const string type)
        { _type = type;  }
       
        string getType() const
        { return _type; }

        void setUsed(int val)
        { _used = val; }

        int getUsed()
        { return _used; }

        void decompile(ostream &os) const
        {
        os<<_name;
        }

        void decompiletoyices(ostream &os) 
        {
        os<<"(define "<<_name<<"::"<<_type<<")"<<endl;
        }
};

#endif
