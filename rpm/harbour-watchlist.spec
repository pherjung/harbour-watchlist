# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       harbour-watchlist

# >> macros
# << macros

Summary:    Sailfish OS Stock Watchlist Application
Version:    0.9.2
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/andywuest/harbour-watchlist
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-watchlist.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  desktop-file-utils

%description
Watchlist is a stock watchlist application for Sailfish OS


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
export VERSION_NUMBER=%{version}
# << build pre

%qmake5 

make %{?_smp_mflags}

# >> build post
echo "Strip the binary!"
strip %{name}
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
# >> files
# << files
