/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.hpp"

using namespace fakeit;


struct DefaultErrorFormatting: tpunit::TestFixture {
	DefaultErrorFormatting() :
			tpunit::TestFixture(
			//
			TEST(DefaultErrorFormatting::calling_an_unstubbed_method_should_raise_UnmockedMethodCallException),
			TEST(DefaultErrorFormatting::check_atleast_verification_exception),
			TEST(DefaultErrorFormatting::check_exact_verification_exception),
			TEST(DefaultErrorFormatting::check_no_more_invocations_exception)
			) //
	{
	}

	template <typename T> std::string to_string(T& val){
		std::stringstream stream;
		stream << val;
		return stream.str();
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
	};
	void calling_an_unstubbed_method_should_raise_UnmockedMethodCallException() {
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		try {
			i.func(1);
			FAIL();
		}
		catch (UnexpectedMethodCallException& e)
		{
			std::string expectedMsg("UnexpectedMethodCallException: could not find any recorded behavior to support this method call");
			ASSERT_EQUAL(expectedMsg, to_string(e));
		}
	}

	void check_atleast_verification_exception() {
		Mock<SomeInterface> mock;
		try {
			Verify(Method(mock, func));
		}
		catch (SequenceVerificationException& e) {
			std::string expected{ "VerificationException: expected at least 1 invocations but was 0" };
			ASSERT_EQUAL(expected, to_string(e));
		}
	}


	void check_exact_verification_exception() {
		Mock<SomeInterface> mock;
		try {
			Verify(Method(mock, func)).Exactly(Once);
		}
		catch (SequenceVerificationException& e) {
			std::string expected{ "VerificationException: expected exactly 1 invocations but was 0" };
			ASSERT_EQUAL(expected, to_string(e));
		}
	}

	void check_no_more_invocations_exception() {
		Mock<SomeInterface> mock;
		try {
			Fake(Method(mock, func));
			mock.get().func(1);
			mock.get().func(2);
			Verify(Method(mock, func).Using(1));
			VerifyNoOtherInvocations(Method(mock, func));
		}
		catch (NoMoreInvocationsVerificationException& e) {
			std::string expected{ "VerificationException: expected no more invocations but found 1" };
			ASSERT_EQUAL(expected, to_string(e));
		}
	}

} __DefaultErrorFormatting;
