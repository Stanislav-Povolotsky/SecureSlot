/**
 * ZeroNight Vuln Smart Contract
 * The idea is to make vuln contract special for ZeroNight on EOS :)
 *
 * @author @s5aava
 */
#include <vector>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/print.hpp>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>

using eosio::key256;
using eosio::indexed_by;
using eosio::const_mem_fun;
using eosio::asset;
using eosio::vector;
using eosio::permission_level;
using eosio::unpack_action_data;
using eosio::action;
using eosio::name;
using eosio::symbol_type;
using eosio::string_to_symbol;
using eosio::print;
using std::string;
using std::to_string;

#define CONTRACT_ACCOUNT_NAME "zeronighteos"

/* tables */
#include "tables/tb_seed.hpp"

/* controllers */
#include "controllers/main_control.hpp"
#include "controllers/withdraw_control.hpp"


class zeronight : 	public eosio::contract,
				    public main_control {

private:
    withdraw_control withdraw_controller;

public:
    zeronight(account_name self)
    : eosio::contract(self)
    , withdraw_controller(self)
    {}

    /// @abi action
	void transfer(account_name from, account_name to, asset price, string memo){
		withdraw_controller.ntransfer( from, to, price, memo );
	}

};

/* ================================================================= */

// extend from EOSIO_ABI
#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( action == N(onerror)) { \
         /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
         eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
      } \
      if( code == self || code == N(eosio.token) || action == N(onerror) ) { \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}

EOSIO_ABI_EX(zeronight, (transfer))