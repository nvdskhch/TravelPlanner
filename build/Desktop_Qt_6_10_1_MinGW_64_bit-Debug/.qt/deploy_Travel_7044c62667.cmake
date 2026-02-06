include("C:/QtProjects/Travel/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Travel-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/QtProjects/Travel/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/Travel.exe"
    GENERATE_QT_CONF
)
