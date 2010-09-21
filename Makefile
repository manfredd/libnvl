#/////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
#//
#// Distributed under the Boost Software License, Version 1.0.
#// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#/////////////////////////////////////////////////////////////////////////////////////////////


INSTALL_PATH ?=


all:
	@$(MAKE) -C nicta_common
	@$(MAKE) -C static_association
	@$(MAKE) -C thread
	@$(MAKE) -C image_header
	@$(MAKE) -C avi_header
	@$(MAKE) -C fwd
	@$(MAKE) -C image
	@$(MAKE) -C video
	@$(MAKE) -C ocv_matrix
	@$(MAKE) -C ocv_converters


install:
	@$(MAKE) -C nicta_common install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C static_association install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C thread install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C image_header install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C avi_header install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C fwd install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C image install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C video install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C ocv_matrix install INSTALL_PATH=$(INSTALL_PATH)
	@$(MAKE) -C ocv_converters install INSTALL_PATH=$(INSTALL_PATH)


clean:
	@echo "  cleaning..."


clobber: clean
	@echo "  clobbering..."
	@$(MAKE) -C nicta_common clobber
	@$(MAKE) -C static_association clobber
	@$(MAKE) -C thread clobber
	@$(MAKE) -C image_header clobber
	@$(MAKE) -C avi_header clobber
	@$(MAKE) -C fwd clobber
	@$(MAKE) -C image clobber
	@$(MAKE) -C video clobber
	@$(MAKE) -C ocv_matrix clobber
	@$(MAKE) -C ocv_converters clobber
