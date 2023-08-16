
find_package(Qt6 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Xml Test)

include(ECMAddTests)

ecm_add_tests(
   kactioncategorytest.cpp
   kactioncollectiontest.cpp
   kxmlgui5configmigrationtest.cpp
   LINK_LIBRARIES Qt6::Test KF6::XmlGui
)

target_compile_definitions(kxmlgui5configmigrationtest PRIVATE -DKXMLGUI5CONFIGMIGRATION_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )
set_tests_properties(kxmlgui5configmigrationtest PROPERTIES RUN_SERIAL TRUE) # wipes out ~/.qttest/{config,share}

ecm_add_tests(
   kmainwindow_unittest.cpp
   ktoolbar_unittest.cpp
   ktooltiphelper_unittest.cpp
   kxmlgui_unittest.cpp
   GUI
   LINK_LIBRARIES Qt6::Test KF6::IconThemes KF6::XmlGui
)

set_tests_properties(ktoolbar_unittest PROPERTIES RUN_SERIAL TRUE) # it wipes out ~/.qttest/share
