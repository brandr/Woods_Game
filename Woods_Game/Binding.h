#ifndef BINDING_H
#define BINDING_H

#include <map>
#include <memory>

class BindingInterface
{
	int m_Size;
	~BindingInterface() = default;
};

template <typename T>
class Binding : public BindingInterface {
	T m_Value;
};

struct BindingSet {
	std::map<std::string, std::unique_ptr<BindingInterface> > fields;
}

#endif // ! BINDING_H
