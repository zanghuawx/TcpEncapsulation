#ifndef ANY_H
#define ANY_H

#include <typeinfo>
#include <string>
#include <iostream>
#include <list>
#include <cassert>

class any {
public:
  class placeholder {
  public:
    virtual ~placeholder() {

    }
  public:
    virtual const std::type_info& type() const = 0;
    virtual placeholder* clone() const = 0;
  };

  template<typename ValueType>
  class holder : public placeholder {
  public:
    holder(const ValueType& value): held(value) {

    }
  public:
    virtual const std::type_info& type() const {
      return typeid(ValueType);
    }

    virtual placeholder* clone() const {
      return new holder(held);
    }
  public:
    ValueType held;
  };

public:
  any(): content(NULL) {

  }

  //模板构造函数，参数可以是任意类型，真正的数据保存在content中   
  template<typename ValueType> 
  any(const ValueType& value): content(new holder<ValueType>(value)) {

  }

  //拷贝构造函数
  any(const any& other): content(other.content ? other.content->clone() : 0) {

  }

  ~any() {
    if (NULL != content) {
      delete content;
    }
  }
  placeholder* content;

private:
  
  template<typename ValueType>
  friend ValueType any_cast(const any* operand);

public:
  const std::type_info& type() const {
    return content ? content->type() : typeid(void);
  }
};

template<typename ValueType>
ValueType any_cast(const any& operand) {
  assert(operand.type() == typeid(ValueType));
  return static_cast<any::holder<ValueType>*>(operand.content)->held;
}



#endif // ANY_H

