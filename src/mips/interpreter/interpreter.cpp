#include <mips/interpreter/interpreter.hpp>
#include <mips/interpreter/exception/interpreter_exception.hpp>
#include <mips/interpreter/encoder/encoder_factory.hpp>
#include <mips/interpreter/encoder/encoder.hpp>
#include <mips/util/filter/space_filter.hpp>
#include <mips/core.hpp>
#include <string>
#include <cstring>

using namespace MIPS;

Interpreter::Interpreter(const char* file) {
	SpaceFilter spaceFilter;
	fileReader = new FileReader(file, spaceFilter);
	errors = 0;
}

Interpreter::~Interpreter() {
	delete fileReader;
}

void Interpreter::process() {
	char *line = NULL;
	char *token = NULL;
	while (fileReader->hasNext()) {
		std::vector<char*> tokens;
		line = fileReader->next();
		token = strtok(line, " ,");
		while (token != NULL) {
			if (strlen(token) > 0) {
				tokens.push_back(token);
			}
			token = strtok(NULL, " ,");
		}
		lines.push_back(tokens);
	}
	// Substitui os labels pelo numero da linha
	this->updateLabels();
	try {
		// Processa as linhas de assembly pra transformar em codigo de máquina.
		this->convertToInstructions();
	} catch (InterpreterException& e) {
		++errors;
		throw e;
	}
}

void Interpreter::updateLabels() {
	char* buffer, *endBuffer;
	char labelBuffer[64];
	unsigned long i = 0;
	while (i < lines.size()) {
		buffer = lines.at(i).at(0);
		if ((endBuffer = strchr(buffer, ':')) != NULL) {
			strncpy(labelBuffer, buffer, endBuffer - buffer);
			labelBuffer[endBuffer - buffer] = '\0';
			// Salva o label temporariamente para poder substituir suas ocorrencias
			// pelo numero da instrução que deverá ser executada.
			Label label;
			strcpy(label.label, labelBuffer);
			label.line = i;
			labels.push_back(label);
			// Remove o label
			lines.at(i).erase(lines.at(i).begin());
			// Checa se existe algo na linha, senao deleta a linha
			if (lines.at(i).size() == 0) {
				lines.erase(lines.begin() + i);
				--i;
			}
		}
		++i;
	}
}

void Interpreter::convertToInstructions() {
	EncoderFactory factory(labels);
	unsigned long size = lines.size();
	for (unsigned long i = 0; i < size; ++i) {
		char *operation = lines.at(i).at(0);
		Encoder* encoder = factory.produce(operation);
		instruction32_t instruction = encoder->encode(lines.at(i));
	}
}
