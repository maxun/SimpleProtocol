enable_testing()

SET(KIT SimpleProtocol)
CREATE_TEST_SOURCELIST(Tests ${KIT}CxxTests.cxx
  PacketTest.cpp
  DecoderTest.cpp
  EncoderTest.cpp
  PacketPoolTest.cpp
)

ADD_EXECUTABLE(${KIT}CxxTests ${Tests} ${Sources})
target_compile_definitions(SimpleProtocol PRIVATE SIMPLEPROTOCOL_STATIC )
SET (TestsToRun ${Tests})
REMOVE (TestsToRun ${KIT}CxxTests.cxx)

FOREACH (test ${TestsToRun})
  GET_FILENAME_COMPONENT(TName ${test} NAME_WE)
  ADD_TEST(${TName} ${CXX_TEST_PATH}/${KIT}CxxTests ${TName})
ENDFOREACH (test)
