#/////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
#//
#// Distributed under the Boost Software License, Version 1.0.
#// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#/////////////////////////////////////////////////////////////////////////////////////////////


INSTALL_PATH ?= 


all:
	@$(MAKE) -C guard
	@$(MAKE) -C multiton
	@$(MAKE) -C condition
	@$(MAKE) -C rw_locks
	@$(MAKE) -C gate


install:
	@$(MAKE) -C guard install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C multiton install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C condition install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C rw_locks install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C gate install INSTALL_PATH=$(INSTALL_PATH)


clean:
	@echo "  cleaning..."


clobber: clean
	@echo "  clobbering..."
	@rm -rf include
