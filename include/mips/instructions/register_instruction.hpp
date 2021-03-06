/**
 * \file register_instruction.hpp
 *
 * Arquivo contendo uma classe que representa uma instrução do tipo (R)egister.
 */
#pragma once

#include <mips/core.hpp>
#include <mips/instructions/instruction.hpp>

class Register;

namespace MIPS {

/**
 * Classe que representa uma instrução do tipo (R)egister.
 *
 * \author Matheus Nogueira
 */
class RegisterInstruction : public Instruction {

public:

    /**
     * Cria uma nova instrução (R)egister.
     *
     * \param opcode codigo da operação
     * \param rs registrador source
     * \param rt registrador target
     * \param rd registrador destination
     * \param shamt quantidade de shift
     * \param funct bits para escolha da função da instrução
     */
    RegisterInstruction(bit8_t opcode,
                        Register *rs,
                        Register *rd,
                        Register *rt,
                        bit8_t shamt,
                        bit8_t funct);

    /**
     * Destroi a instrução.
     */
     virtual ~RegisterInstruction();

protected:

    /**
     * Registrador source (Rs) da instrução.
     */
    Register *rs;

    /**
     * Registrador target (Rt) da instrução.
     */
    Register *rt;

    /**
     * Registrador destination (Rd) da instrução.
     */
    Register *rd;

    /**
     * Valor do shamt (shift amount) da instrução.
     */
    bit8_t shamt;

    /**
     * Valor da funct da instrução.
     */
    bit8_t funct;

};

}; // namespace
