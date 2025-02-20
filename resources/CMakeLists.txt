# Author : Anthony Geay (EDF R&D)

INCLUDE(UseQtExt)
SET(_xml_files
  OPENTURNSCatalog.xml
  )
  
SET(_png_files
  OT.png
  document-open.png
  document-save.png
  document-export.png
  document-import.png
  document-save-as.png
  document-new22x22.png
  )

SET(_res_files
  ${_png_files}
  ${_xml_files}
  )

SET(_ts_files
  OPENTURNS_msg_en.ts
  OPENTURNS_images.ts
  )

SALOME_CONFIGURE_FILE(SalomeAppSL.xml.in SalomeAppSL.xml INSTALL ${OTSALOMEMODULE_RES_DIR})

SET(OTSALOMEMODULE_RES_DIR share/salome/resources/openturns)
INSTALL(FILES ${_res_files} DESTINATION ${OTSALOMEMODULE_RES_DIR})
QT_INSTALL_TS_RESOURCES("${_ts_files}" "${OTSALOMEMODULE_RES_DIR}")
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/SalomeAppSL.xml DESTINATION ${OTSALOMEMODULE_RES_DIR})
