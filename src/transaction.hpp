#pragma once
#include "units.hpp"
#include "address.hpp"
#include "proof_of_work.hpp"
#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/sha224.hpp>
#include <fc/io/varint.hpp>


struct output_reference
{
  fc::sha224        trx_hash;   // the hash of a transaction.
  fc::unsigned_int  output_idx; // the output index in the transaction trx_hash
};
FC_REFLECT( output_reference, (trx_hash)(output_idx) )

enum claim_type
{
   /** basic claim by single address */
   claim_by_address = 0,
};
FC_REFLECT_ENUM( claim_type, (claim_by_address) )


/**
 *  Defines the source of an input used 
 *  as part of a transaction.  If must first
 *  reference an existing unspent output which
 *  it can do in one of two ways:
 *
 *  1) provide the index in the deterministic unspent output
 *     table, or provide the hash of the transaction and the
 *     index of the output.
 *
 *  2) provide the hash of the transaction and the output index.
 *
 *  Any unspent output that is older than 24 hours can safely
 *  be addressed using the unique id assigned in the unspent
 *  output table.  Transactions that reference 'newer' outputs
 *  will have to reference the Transaction directly to survive
 *  a potential reorganization.
 */
struct trx_input
{
    /** 0 for output index, 1 for output ref */
    uint8_t                          output_flags;
    fc::optional<uint64_t>           output_index;
    fc::optional<output_reference>   output_ref;
    claim_type                       output_type;
};
FC_REFLECT( trx_input, (output_flags)(output_index)(output_ref) )


/**
 *   An input that references an output that can be claimed by
 *   an address.  A compact signature is used that when combined
 *   with the hash of the transaction containing this input will
 *   allow the public key and therefore address to be discovered and
 *   validated against the output claim function.
 */
struct trx_input_by_address : public trx_input
{
   enum type_enum { type =  claim_type::claim_by_address };
   fc::ecc::compact_signature address_sig;
};
FC_REFLECT_DERIVED( trx_input_by_address, (trx_input), (address_sig) );


/**
 *  Base of all trx outputs, derived classes will define the extra
 *  data associated with the claim_func.  The goal is to enable the
 *  output types to be 'fixed size' and thus each output type would
 *  have its own 'memory pool' in the chain state data structure. This
 *  has the added benefit of conserving space, separating bids/asks/
 *  escrow/and normal transfers into different memory segments and
 *  should give better memory performance.   
 */
struct trx_output
{
    uint64_t       amount;
    bond_type      unit;
    claim_type     claim_func;
};
FC_REFLECT( trx_output, (amount)(unit)(claim_func) )

/**
 *  Used by normal bitcoin-style outputs spendable with
 *  the signature of the private key.
 */
struct trx_output_by_address : public trx_output
{
   enum type_enum { type =  claim_type::claim_by_address };
   address  claim_address;  // the address that can claim this input.
};
FC_REFLECT_DERIVED( trx_output_by_address, (trx_output), (claim_address) )


/**
 *  Holds any type of transaction input in the data field.
 */
struct generic_trx_in
{
  uint8_t           in_type;
  std::vector<char> data;
};
FC_REFLECT( generic_trx_in, (in_type)(data) )

struct generic_trx_out
{
  uint8_t           out_type;
  std::vector<char> data;
};
FC_REFLECT( generic_trx_out, (out_type)(data) )

/**
 *  @brief maps inputs to outputs.
 */
struct transaction
{
   uint16_t                     version;
   uint32_t                     expire_block;
   std::vector<generic_trx_in>  inputs;
   std::vector<generic_trx_out> outputs;
};
FC_REFLECT( transaction, (version)(inputs)(outputs) )

struct signed_transaction : public transaction
{
    std::vector<fc::ecc::compact_signature> sigs;
};
FC_REFLECT_DERIVED( signed_transaction, (transaction), (sigs) )

