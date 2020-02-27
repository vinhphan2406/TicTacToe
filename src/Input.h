#include "GameObj.h"

#include <fstream>
#include <unordered_map>

#ifndef I_INPUT_H
#define I_INPUT_H

// enum class Type {
//     INT,
//     ICOMMAND,
//     POSITION,
//     NONE,
//     NUM_OF_TYPES
// };

// std::unordered_map<std::string, Type> typeString = {
//     {"int", Type::INT},
//     {"ICommand", Type::ICOMMAND},
//     {"Position", Type::POSITION},
//     {"None", Type::NONE}
// };

struct ObjectBase {
    virtual ~ObjectBase(){}
    virtual ObjectBase* clone() const = 0;
};

template<class T>
struct ObjectTemplate : ObjectBase {
    ObjectTemplate(): value(){}
    ObjectTemplate(const T& _val): value(_val){}
    virtual ObjectBase* clone() const override {
        return new ObjectTemplate<T>(this->value);
    }
    T value;
};

class Object {
    public:
        Object(): holder(NULL){}
        template<class T>
        Object(const T& _val){
            this->holder = new ObjectTemplate<T>(_val);
        }
        Object(const Object& _obj): holder(_obj.holder->clone()){}
        ~Object(){
            this->clear();
        }

        template<class T>
        void setObjectValue(const T& _val) {
            this->clear();
            this->holder = new ObjectTemplate<T>(_val);
        }

        template<class T>
        const T& getObjectValue() const {
            return static_cast<ObjectTemplate<T>*>(this->holder)->value;
        }
        
        void clear(){
            if (this->holder) delete this->holder;
        }
    private:
        ObjectBase* holder;
};


class IInput {
    public:
        virtual ~IInput() = default;
        virtual Object input() = 0;
};
#endif