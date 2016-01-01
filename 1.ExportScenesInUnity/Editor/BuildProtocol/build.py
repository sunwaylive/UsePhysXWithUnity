#! /usr/bin/env python
#coding=utf-8

import os
import sys
import string 

if __name__ == '__main__' :
	
	PROTO_PATH=os.path.join(".","Protocol")
	STEP1_PROTO2CS_PATH=os.path.join(".","step1_proto2cs")
	PROTO_DESC_POSTFIX=".protodesc"
	
	print "=========Compilation of protocol in " + STEP1_PROTO2CS_PATH + " ========="
	
#---------------------------------------------------
#step1 : remove temp file -> proto gen cs
#---------------------------------------------------
	os.chdir(STEP1_PROTO2CS_PATH)
	print("current run dir : %s" %os.getcwd())
	print "TRY TO DELETE TEMP FILES IN STEP1:"		

	def del_files_in_step1(dir,topdown=True):
		abs_dir = os.path.abspath(dir)
		for root, dirs, files in os.walk(dir, topdown):			
			for name in files: 				
				if ((root == abs_dir) and (-1 != name.find(".cs") or -1 != name.find(".protodesc") or -1 != name.find(".txt"))):
					os.remove(name)
					print("del file : %s" %name)
			if root != abs_dir :				
				break
	
	del_files_in_step1(os.getcwd())	
	print("del end")
	
	proto_file_list = []
	proto_dir = os.path.join("..",PROTO_PATH)
	for root, dirs, files in os.walk(proto_dir, True):             	
			for name in files:  
				if (-1 != name.find(".proto")):
					fileName,fileExt = os.path.splitext(os.path.basename(name))
					proto_file_list.append(fileName)
					print("proto file : %s" %fileName)
	
	for proto_file in proto_file_list:
		print proto_file
		protoc_command = os.path.join(".","protoc --descriptor_set_out=" + proto_file + PROTO_DESC_POSTFIX + " --proto_path=..",PROTO_PATH + " ..",PROTO_PATH,proto_file + ".proto")
		print "command ====== " + protoc_command
		os.system(protoc_command)
		
		protogen_command = os.path.join("mono ProtoGen","protogen.exe -i:" + proto_file + PROTO_DESC_POSTFIX + " -o:" + proto_file + ".cs")
		print "command ====== " + protogen_command
		os.system(protogen_command)

	os.chdir("..")
	print("step2 current run dir : %s" %os.getcwd())
	
#---------------------------------------------------
#step2 : data and cs to assets dir
#---------------------------------------------------
	OUT_PATH=os.path.join("..","Assets")
	CS_DEST=os.path.join("Scripts","Killer","Protocol","ProtoGen")
	
	def CopyFileAndRename(srcFile,desFile):
		if(os.path.isfile(srcFile)):
			open(desFile,"wb").write(open(srcFile, "rb").read())
	
	def GetFileNameList(dir,ext,topdown=True):
		proto_file_list = []
		for root, dirs, files in os.walk(dir, topdown):             	
			for name in files:  
				if (-1 != name.find(ext)):
					fileName,fileExt = os.path.splitext(os.path.basename(name))
					proto_file_list.append(fileName)
		return proto_file_list
		
	csList = GetFileNameList(STEP1_PROTO2CS_PATH,".cs")
	for cs in csList :
		print(os.path.join(STEP1_PROTO2CS_PATH,cs + ".cs -> ") + os.path.join(OUT_PATH,CS_DEST,cs + ".cs"))
		CopyFileAndRename(os.path.join(STEP1_PROTO2CS_PATH,cs + ".cs"),os.path.join(OUT_PATH,CS_DEST,cs + ".cs"))
		

#---------------------------------------------------
#step3 : do del
#---------------------------------------------------

	print "TRY TO DEL TEMP FILE:"
	print "step1:"
	os.chdir(STEP1_PROTO2CS_PATH)
	del_files_in_step1(os.getcwd())
	
	os.chdir("..")
		
	
	
	
	
	
	

	

		
	
	
	
	
	
	



