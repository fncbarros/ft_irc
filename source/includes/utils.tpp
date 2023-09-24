/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:11:15 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/14 18:11:36 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ostream>
#include <string>

namespace Utils
{
	template<typename T>
	std::string numToStr(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	template<typename T>
	T strToNum(const std::string& str)
	{
		std::istringstream iss(str);
		T converted;
		iss >> converted;
		return converted;
	}

	template<typename From, typename To>
	To riskyConversion(const From& value)
	{
		std::istringstream iss(value);
		To converted;
		try {
			iss >> converted;
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		return converted;
	}
} // namespace Utils
