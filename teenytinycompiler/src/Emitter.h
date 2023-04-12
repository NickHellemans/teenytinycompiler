#pragma once
#include <fstream>
#include <string>

class Emitter
{
public:
	explicit Emitter(const std::string& filepath)
	{
		this->filepath = filepath;
	}

	void emit(const std::string& codeToAdd)
	{
		this->code += codeToAdd;
	}

	void emitLine(const std::string& codeToAdd)
	{
		this->code += codeToAdd + '\n';
	}

	void emitHeaderLine(const std::string& codeToAdd)
	{
		this->header += codeToAdd + '\n';
	}

	void writeFile() const 
	{
		std::ofstream out(filepath);
		out << header;
		out << code;
		out.close();
	}


private:
	std::string filepath;
	std::string header;
	std::string code;
};
