Name:       elm-modules
Summary:    The Elementary Modules
Version:    0.1.1
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
Source0:    %{name}-%{version}.tar.gz
BuildRequires: elementary-devel
BuildRequires: tts-devel

%description
The Eelementary Modules

%prep
%setup -q

%build

%define DEF_SUBDIRS access_output_tts

export CFLAGS+=" -Wall -g -fPIC -rdynamic"
export LDFLAGS+=" -Wl,--hash-style=both -Wl,--as-needed -Wl,--rpath=/usr/lib"

%ifarch %{arm}
export CFLAGS+=" -D_ENV_ARM"
%endif

for FILE in %{DEF_SUBDIRS}
do
        (cd $FILE && ./autogen.sh && ./configure --prefix=/usr && make )
done

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}

for FILE in %{DEF_SUBDIRS}
do
        (cd $FILE && make install DESTDIR=%{buildroot} )
done

%files
%defattr(-,root,root,-)
/usr/lib/elementary/modules/*/*/*.so
/usr/share/license/%{name}
%manifest %{name}.manifest

%exclude /usr/lib/elementary/modules/*/*/*.la
