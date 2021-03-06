project( bitshares )
cmake_minimum_required( VERSION 2.8.0 )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/fc/CMakeModules;${CMAKE_MODULE_PATH}" )
FIND_PACKAGE( OpenSSL )
SET( ALL_OPENSSL_LIBRARIES
       ${OPENSSL_LIBRARIES}
       ${SSL_EAY_RELEASE}
       ${LIB_EAY_RELEASE}
   )

include( VersionMacros )
include( SetupTargetMacros )

IF( WIN32 )
    ADD_DEFINITIONS( -DWIN32 )
	ADD_DEFINITIONS( -DBOOST_CONTEXT_NO_LIB )
	ADD_DEFINITIONS( -D_SCL_SECURE_NO_WARNINGS )
	ADD_DEFINITIONS( -D_WIN32_WINNT=0x0501 )
	ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )
	ADD_DEFINITIONS( -DBOOST_ALL_NO_LIB -DBOOST_THREAD_BUILD_LIB)
	ADD_DEFINITIONS( -DWIN32)
	# Activate C++ exception handling
	IF (NOT CMAKE_CXX_FLAGS MATCHES "/EHsc")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")    
	ENDIF()
ELSE(WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wno-unused-local-typedefs -fmax-errors=3 -msse4.2" )
ENDIF(WIN32)


set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.51 COMPONENTS thread date_time system filesystem program_options signals serialization chrono unit_test_framework context ) 

include_directories( ${CMAKE_SOURCE_DIR}/fc/include )
include_directories( ${CMAKE_SOURCE_DIR}/fc/vendor/boost_1.51/include )
include_directories( ${Boost_INCLUDE_DIR} )
   
#link_directories( ${Boost_LIBRARY_DIRS} )

add_subdirectory( fc )
set( BOOST_LIBRARIES ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_CHRONO_LIBRARY} ${ALL_OPENSSL_LIBRARIES})


set( sources 
#     src/database.cpp
#     src/block_chain.cpp
     src/address.cpp
     src/wallet.cpp
#     src/account.cpp
#     src/miner.cpp
#     src/chain_state.cpp
     vendor/cityhash-1.0.3/src/city.cc
     src/proof_of_work.cpp )

add_library( bshare ${sources} )

add_executable( pow_test tests/pow_test.cpp )
target_link_libraries( pow_test bshare fc ${BOOST_LIBRARIES})

add_executable( wallet_test tests/wallet_test.cpp )
target_link_libraries( wallet_test bshare fc ${BOOST_LIBRARIES})
#setup_library( bshare SOURCES ${sources} LIBRARY_TYPE STATIC )

#add_executable( unit_tests tests/unit_tests.cpp )
#target_link_libraries( unit_tests bshare fc ${BOOST_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}  )

#add_executable( bitshares src/main.cpp )
#target_link_libraries( bitshares bshare fc ${BOOST_LIBRARIES})
