# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# This file is part of the LibreOffice project.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

$(eval $(call gb_Library_Library,odfgen))

$(eval $(call gb_Library_use_unpacked,odfgen,libodfgen))

$(eval $(call gb_Library_use_externals,odfgen,\
    boost_headers \
	etonyek \
	wpd \
	wpg \
))

$(eval $(call gb_Library_set_warnings_not_errors,odfgen))

$(eval $(call gb_Library_set_include,odfgen,\
    -I$(call gb_UnpackedTarball_get_dir,libodfgen)/inc \
    $$(INCLUDE) \
))

$(eval $(call gb_Library_add_defs,odfgen,\
	-DBOOST_ALL_NO_LIB \
	-DDLL_EXPORT \
	-DLIBODFGEN_BUILD \
	-DNDEBUG \
))

$(eval $(call gb_Library_add_generated_exception_objects,odfgen,\
    UnpackedTarball/libodfgen/src/DocumentElement \
    UnpackedTarball/libodfgen/src/FontStyle \
    UnpackedTarball/libodfgen/src/GraphicFunctions \
    UnpackedTarball/libodfgen/src/InternalHandler \
    UnpackedTarball/libodfgen/src/ListStyle \
    UnpackedTarball/libodfgen/src/OdgGenerator \
    UnpackedTarball/libodfgen/src/OdpGenerator \
    UnpackedTarball/libodfgen/src/OdtGenerator \
    UnpackedTarball/libodfgen/src/PageSpan \
    UnpackedTarball/libodfgen/src/SectionStyle \
    UnpackedTarball/libodfgen/src/TableStyle \
    UnpackedTarball/libodfgen/src/TextRunStyle \
))

# vim: set noet sw=4 ts=4:
