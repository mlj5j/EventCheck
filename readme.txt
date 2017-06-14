1) TO SET UP CODE:
-----------------

setenv SCRAM_ARCH slc6_amd64_gcc493

cmsrel CMSSW_7_6_3

cd CMSSW_7_6_3/src

cmsenv

git-cms-addpkg JetMETCorrections/MCJet

cd JetMETCorrections/MCJet

rm -fr plugins/

cp  /afs/cern.ch/user/p/petyt/public/EventCheck/code/plugins.zip .

scramv1 b

cp /afs/cern.ch/user/p/petyt/public/EventCheck/code/datacheck_cfg.py  test/


2) TO RUN CODE:
--------------

nice cmsRun test/datacheck_cfg.py





