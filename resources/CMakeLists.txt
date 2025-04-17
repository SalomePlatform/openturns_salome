# Author : Anthony Geay (EDF R&D)

INCLUDE(UseQtExt)
SET(_xml_files
  SalomeApp.xml
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

SET(OTSALOMEMODULE_RES_DIR share/salome/resources/openturns)
INSTALL(FILES ${_res_files} DESTINATION ${OTSALOMEMODULE_RES_DIR})
QT_INSTALL_TS_RESOURCES("${_ts_files}" "${OTSALOMEMODULE_RES_DIR}")
INSTALL(FILES SalomeApp.xml RENAME SalomeAppSL.xml DESTINATION ${OTSALOMEMODULE_RES_DIR})
