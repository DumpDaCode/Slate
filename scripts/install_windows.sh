pacman -S mingw-w64-x86_64-gcc --noconfirm --needed
pacman -S make --noconfirm --needed
pacman -S mingw-w64-x86_64-pkg-config --noconfirm --needed
pacman -S mingw-w64-x86_64-gtk3 --noconfirm --needed

cp -r -u ../theme/. /mingw64/share/themes/Windows10/
touch /mingw64/etc/gtk-3.0/settings.ini

echo "[Settings]
gtk-theme-name=Windows10
gtk-font-name=Segoe UI 9" >  /mingw64/etc/gtk-3.0/settings.ini

cd ../src
make slate-windows