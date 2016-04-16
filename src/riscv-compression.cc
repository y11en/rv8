//
//  riscv-compression.h
//

#include <map>

#include "riscv-types.h"
#include "riscv-format.h"
#include "riscv-opcodes.h"
#include "riscv-imm.h"
#include "riscv-regs.h"
#include "riscv-decode.h"
#include "riscv-compression.h"

/* Compression Meta Data */

const riscv_inst_comp_metadata riscv_comp_table[] = {
    { riscv_op_c_addi,     riscv_inst_type_ci,            riscv_op_addi,       riscv_inst_type_i },
    { riscv_op_c_lw,       riscv_inst_type_cl_lw,         riscv_op_lw,         riscv_inst_type_i },
    { riscv_op_c_mv,       riscv_inst_type_cr,            riscv_op_addi,       riscv_inst_type_i },
    { riscv_op_c_bnez,     riscv_inst_type_cb,            riscv_op_bne,        riscv_inst_type_sb },
    { riscv_op_c_sw,       riscv_inst_type_cs_sw,         riscv_op_sw,         riscv_inst_type_s },
    { riscv_op_c_ld,       riscv_inst_type_cl_ld,         riscv_op_ld,         riscv_inst_type_i },
    { riscv_op_c_swsp,     riscv_inst_type_css_swsp,      riscv_op_sw,         riscv_inst_type_s },
    { riscv_op_c_lwsp,     riscv_inst_type_ci_lwsp,       riscv_op_lw,         riscv_inst_type_i },
    { riscv_op_c_li,       riscv_inst_type_ci_li,         riscv_op_addi,       riscv_inst_type_i },
    { riscv_op_c_add,      riscv_inst_type_cr,            riscv_op_add,        riscv_inst_type_r },
    { riscv_op_c_srli,     riscv_inst_type_cb,            riscv_op_srli_rv64i, riscv_inst_type_i },
    { riscv_op_c_jr,       riscv_inst_type_cr,            riscv_op_jalr,       riscv_inst_type_i },
    { riscv_op_c_fld,      riscv_inst_type_cl_ld,         riscv_op_fld,        riscv_inst_type_i },
    { riscv_op_c_sdsp,     riscv_inst_type_css_sdsp,      riscv_op_sd,         riscv_inst_type_s },
    { riscv_op_c_j,        riscv_inst_type_cj,            riscv_op_jal,        riscv_inst_type_uj },
    { riscv_op_c_ldsp,     riscv_inst_type_ci_ldsp,       riscv_op_ld,         riscv_inst_type_i },
    { riscv_op_c_andi,     riscv_inst_type_cb,            riscv_op_andi,       riscv_inst_type_i },
    { riscv_op_c_addiw,    riscv_inst_type_ci,            riscv_op_addiw,      riscv_inst_type_i },
    { riscv_op_c_slli,     riscv_inst_type_ci,            riscv_op_slli_rv64i, riscv_inst_type_i },
    { riscv_op_c_sd,       riscv_inst_type_cs_sd,         riscv_op_sd,         riscv_inst_type_s },
    { riscv_op_c_beqz,     riscv_inst_type_cb,            riscv_op_beq,        riscv_inst_type_sb },
    { riscv_op_c_and,      riscv_inst_type_cs,            riscv_op_and,        riscv_inst_type_r },
    { riscv_op_c_srai,     riscv_inst_type_cb,            riscv_op_srai_rv64i, riscv_inst_type_i },
    { riscv_op_c_jal,      riscv_inst_type_cj,            riscv_op_jal,        riscv_inst_type_uj },
    { riscv_op_c_addi4spn, riscv_inst_type_ciw_4spn,      riscv_op_addi,       riscv_inst_type_i },
    { riscv_op_c_fldsp,    riscv_inst_type_ci_lwsp,       riscv_op_fld,        riscv_inst_type_i },
    { riscv_op_c_addi16sp, riscv_inst_type_ci_16sp,       riscv_op_addi,       riscv_inst_type_i },
    { riscv_op_c_fsd,      riscv_inst_type_cs_sd,         riscv_op_fsd,        riscv_inst_type_s },
    { riscv_op_c_fsdsp,    riscv_inst_type_css_sdsp,      riscv_op_fsd,        riscv_inst_type_s },
    { riscv_op_c_addw,     riscv_inst_type_cs,            riscv_op_addw,       riscv_inst_type_r },
    { riscv_op_c_xor,      riscv_inst_type_cs,            riscv_op_xor,        riscv_inst_type_r },
    { riscv_op_c_or,       riscv_inst_type_cs,            riscv_op_or,         riscv_inst_type_r },
    { riscv_op_c_sub,      riscv_inst_type_cs,            riscv_op_sub,        riscv_inst_type_r },
    { riscv_op_c_lui,      riscv_inst_type_ci_lui,        riscv_op_lui,        riscv_inst_type_u },
    { riscv_op_c_jalr,     riscv_inst_type_cr,            riscv_op_jalr,       riscv_inst_type_i },
    { riscv_op_c_subw,     riscv_inst_type_cs,            riscv_op_subw,       riscv_inst_type_r },
    { riscv_op_c_ebreak,   riscv_inst_type_cr,            riscv_op_sbreak,     riscv_inst_type_none },
    { riscv_op_c_flw,      riscv_inst_type_cl_lw,         riscv_op_flw,        riscv_inst_type_i },
    { riscv_op_c_flwsp,    riscv_inst_type_ci_lwsp,       riscv_op_flw,        riscv_inst_type_i },
    { riscv_op_c_fsw,      riscv_inst_type_cs_sw,         riscv_op_fsw,        riscv_inst_type_s },
    { riscv_op_c_fswsp,    riscv_inst_type_css_swsp,      riscv_op_fsw,        riscv_inst_type_s },
    { riscv_op_unknown,    riscv_inst_type_unknown,       riscv_op_unknown,    riscv_inst_type_unknown }
};

struct riscv_inst_comp_map : std::map<riscv_op,const riscv_inst_comp_metadata*>
{
    riscv_inst_comp_map() {
        for (const auto *ent = riscv_comp_table; ent->cop; ent++)
            (*this)[ent->cop] = ent;
    }
};

const riscv_inst_comp_metadata* riscv_lookup_comp_metadata(riscv_op op)
{
    static riscv_inst_comp_map comp_map;
    return comp_map[op];
}
