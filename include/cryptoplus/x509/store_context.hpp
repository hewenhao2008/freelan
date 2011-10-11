/*
 * libcryptoplus - C++ portable OpenSSL cryptographic wrapper library.
 * Copyright (C) 2010-2011 Julien Kauffmann <julien.kauffmann@freelan.org>
 *
 * This file is part of libcryptoplus.
 *
 * libcryptoplus is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * libcryptoplus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 *
 * If you intend to use libcryptoplus in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

/**
 * \file store_context.hpp
 * \author Julien KAUFFMANN <julien.kauffmann@freelan.org>
 * \brief A X509 store context class.
 */

#ifndef CRYPTOPLUS_X509_STORE_CONTEXT_HPP
#define CRYPTOPLUS_X509_STORE_CONTEXT_HPP

#include "../pointer_wrapper.hpp"
#include "../error/cryptographic_exception.hpp"

#include "store.hpp"
#include "certificate.hpp"

#include <openssl/x509.h>
#include <openssl/x509v3.h>

namespace cryptoplus
{
	namespace x509
	{
		/**
		 * \brief A X509 certificate store context.
		 *
		 * The store class represents a X509 certificate store context.
		 *
		 * A store instance has the same semantic as a X509_STORE_CTX* pointer, thus two copies of the same instance share the same underlying pointer.
		 *
		 * \warning Always check for the object not to be NULL before calling any of its method. Calling any method (except raw()) on a null object has undefined behavior.
		 */
		class store_context: public pointer_wrapper<X509_STORE_CTX>
		{
			public:

				/**
				 * \brief Create a new store context.
				 * \return The store context.
				 *
				 * If allocation fails, a cryptographic_exception is thrown.
				 */
				static store_context create();

				/**
				 * \brief Take ownership of a specified X509_STORE_CTX pointer.
				 * \param ptr The pointer. Cannot be NULL.
				 * \return A store.
				 */
				static store_context take_ownership(pointer ptr);

				/**
				 * \brief Create a new empty X509 store context.
				 */
				store_context();

				/**
				 * \brief Create a X509 store context by *NOT* taking ownership of an existing X509_STORE_CTX* pointer.
				 * \param ptr The X509_STORE_CTX* pointer.
				 * \warning The caller is still responsible for freeing the memory.
				 */
				store_context(pointer ptr);

				/**
				 * \brief Initialize the store context.
				 * \param _store The store to use, if any. Can be NULL.
				 * \param cert The certificate to be verified. Can be NULL.
				 * \param chain Additional untrusted certificates that might be used to build the chain.
				 */
				void initialize(store _store, certificate cert, STACK_OF(X509)* chain);

				/**
				 * \brief Set a trusted stack of certificates.
				 * \param certs The trusted stack of certificates to set.
				 *
				 * This is an alternative way of specifying trusted certificates instead of using a x509::store.
				 */
				void set_trusted_certificates(STACK_OF(X509)* certs);

				/**
				 * \brief Cleanup the store context.
				 *
				 * The context can then be reused and you can call initialize() again.
				 */
				void cleanup();

			private:

				store_context(pointer _ptr, deleter_type _del);
		};

		/**
		 * \brief Compare two store_context instances.
		 * \param lhs The left argument.
		 * \param rhs The right argument.
		 * \return true if the two store_context instances share the same underlying pointer.
		 */
		bool operator==(const store_context& lhs, const store_context& rhs);

		/**
		 * \brief Compare two store_context instances.
		 * \param lhs The left argument.
		 * \param rhs The right argument.
		 * \return true if the two store_context instances do not share the same underlying pointer.
		 */
		bool operator!=(const store_context& lhs, const store_context& rhs);

		inline store_context store_context::create()
		{
			pointer _ptr = X509_STORE_CTX_new();

			error::throw_error_if_not(_ptr);

			return take_ownership(_ptr);
		}
		inline store_context store_context::take_ownership(pointer _ptr)
		{
			error::throw_error_if_not(_ptr);

			return store_context(_ptr, deleter);
		}
		inline store_context::store_context()
		{
		}
		inline store_context::store_context(pointer _ptr) : pointer_wrapper<value_type>(_ptr, null_deleter)
		{
		}
		inline void store_context::initialize(store _store, certificate cert, STACK_OF(X509)* chain)
		{
			error::throw_error_if_not(X509_STORE_CTX_init(raw(), _store.raw(), cert.raw(), chain) != 0);
		}
		inline void store_context::set_trusted_certificates(STACK_OF(X509)* certs)
		{
			X509_STORE_CTX_trusted_stack(raw(), certs);
		}
		inline void store_context::cleanup()
		{
			X509_STORE_CTX_cleanup(raw());
		}
		inline store_context::store_context(pointer _ptr, deleter_type _del) : pointer_wrapper<value_type>(_ptr, _del)
		{
		}
		inline bool operator==(const store_context& lhs, const store_context& rhs)
		{
			return lhs.raw() == rhs.raw();
		}
		inline bool operator!=(const store_context& lhs, const store_context& rhs)
		{
			return lhs.raw() != rhs.raw();
		}
	}
}

#endif /* CRYPTOPLUS_X509_STORE_CONTEXT_HPP */

