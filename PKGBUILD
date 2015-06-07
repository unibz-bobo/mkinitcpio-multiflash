# Maintainer: Julian Sanin <sanin89julian@gmail.com>
# Contributor: Daniele Gadler <daniele.gadler@yahoo.it>

pkgname=mkinitcpio-multiflash
pkgver=1.0.2
pkgrel=1
arch=('armv6h' 'armv7h')
license=('MIT')
depends=('mkinitcpio'
         'mkinitcpio-nfs-utils')
install="${pkgname}.install"
source=('LICENSE'
        'whereisip.c'
        'hooks-multiflash'
        'install-multiflash')
md5sums=('cefb772e5780b0fa3b7bbc09097e0ede'
         'b9afc4d03376a9184a09ee3b169aaff3'
         '1851940c4aa58fc69d3c1256aec97d8a'
         'f58e2bae47d8f8016b434cbf5c682c95')

build() {
  gcc -o whereisip -Os whereisip.c
}

package() {
  install -o root -g root -Dm755 whereisip "${pkgdir}/usr/bin/whereisip"
  install -o root -g root -Dm644 hooks-multiflash "${pkgdir}/usr/lib/initcpio/hooks/multiflash"
  install -o root -g root -Dm644 install-multiflash "${pkgdir}/usr/lib/initcpio/install/multiflash"
  install -o root -g root -Dm644 LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
}
