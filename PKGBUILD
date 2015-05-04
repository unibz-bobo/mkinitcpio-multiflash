# Maintainer: Julian Sanin <sanin89julian@gmail.com>
# Contributor: Daniele Gadler <daniele.gadler@yahoo.it>
pkgname=mkinitcpio-multiflash
pkgver=0.4
pkgrel=1
arch=('armv6h' 'armv7h')
license=('MIT')
depends=('rsync'
         'mkinitcpio'
         'mkinitcpio-nfs-utils')
install="${pkgname}.install"
source=('whereisip.c'
        'hooks-multiflash'
        'install-multiflash')

build() {
  gcc -o whereisip -Os whereisip.c
}

package() {
  install -o root -g root -Dm755 whereisip "${pkgdir}/usr/bin/whereisip"
  install -o root -g root -Dm644 hooks-multiflash "${pkgdir}/usr/lib/initcpio/hooks/multiflash"
  install -o root -g root -Dm644 install-multiflash "${pkgdir}/usr/lib/initcpio/install/multiflash"
}
