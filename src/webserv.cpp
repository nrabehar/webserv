#include "webserv.hpp"

// class B :
//        public virtual IHost<B>
// {
//        public:
// 	       virtual ~B() {}
// 	       virtual void accept(IGuest<B> & visitor) {
// 		       std::cout << "B accepting IGuest<B>" << std::endl;
// 		       visitor.visit(*this);
// 	       }
// };

class B :
       public virtual Host<B, B>
{
       public:
	       virtual ~B() {}
	      //  virtual void accept(IGuest<B> & visitor) {
		    //    std::cout << "B accepting IGuest<B>" << std::endl;
		    //    visitor.visit(*this);
	      //  }
};

class C : public Guest<B> {
public:
       void visit(B &) {
	       std::cout << "C visiting non-const B" << std::endl;
       }
       void visit(const B &) {
	       std::cout << "C visiting const B" << std::endl;
       }
};

int main(int, char **)
{
	B b;
	const B cb;
	C c;
	c.visit(b);
	c.visit(cb);
	b.accept(c, &b)
	return (0);
}
