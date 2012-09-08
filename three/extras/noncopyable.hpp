#ifndef THREE_NONCOPYABLE_HPP
#define THREE_NONCOPYABLE_HPP

namespace three {

class NonCopyable {
protected:
	NonCopyable() {}
	~NonCopyable() {}
private:
	NonCopyable( const NonCopyable& ) = delete;
	const NonCopyable& operator=( const NonCopyable& ) = delete;
};

} // namespace three

#endif // THREE_NONCOPYABLE_HPP
