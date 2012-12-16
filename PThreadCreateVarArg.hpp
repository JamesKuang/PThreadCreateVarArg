#ifndef PTWRAPPER_H
#define PTWRAPPER_H

#include <pthread.h>
#include <assert.h>
#include <tuple>

namespace incyc {

	template <int...>
	struct Sequence {};

	template <int N, int... S>
	struct Gens : Gens<N-1, N-1, S...> {};

	template <int... S>
	struct Gens<0, S...> { typedef Sequence<S...> type; };

	template <typename T, typename R, typename... ATs>
	class Call {
		T *const obj;
		R (T::*fp)(ATs...);
		std::tuple<ATs...> params;
	  public:
		Call(T *o, R (T::*mem)(ATs...), std::tuple<ATs...> p) : obj(o), fp(mem), params(p) {}
		virtual ~Call() {}

		virtual void call_fp() { execute(typename Gens<sizeof...(ATs)>::type()); }

		template <int... S>
		void execute(Sequence<S...>) { (obj->*fp)(std::get<S>(params)...); }
	};

	template <typename T, typename R, typename... ATs>
	static void *stub(void *vp) {
		Call<T, R, ATs...> *call = static_cast<Call<T, R, ATs...> *>(vp);
		call->call_fp();
		delete call;
		return nullptr;
	}

	// User calls this function
	template <typename T, typename R, typename... ATs>
	pthread_t thread_var_arg(T *obj, R (T::*mem)(ATs...), ATs... args) {
		std::tuple<ATs...> params = std::make_tuple(args...);
		Call<T, R, ATs...> *call_arg_p = new Call<T, R, ATs...>(obj, mem, params);
		pthread_t tid = 0;
		int ec = pthread_create(&tid, nullptr, stub<T, R, ATs...>, call_arg_p);
		assert(ec == 0);
		return tid;
	}
}

#endif
