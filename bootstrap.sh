#! /bin/sh
(cd libmoolticute-c;./bootstrap.sh) \
&& aclocal \
&& libtoolize \
&& autoheader \
&& automake --gnu --add-missing \
&& autoconf
