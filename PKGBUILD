# Maintainer: Julian Sanin <sanin89julian@gmail.com>
# Contributor: Daniele Gadler <daniele.gadler@yahoo.it>

pkgname=mkinitcpio-multiflash
pkgver=1.0.4
pkgrel=1
arch=('armv6h' 'armv7h')
license=('MIT')
depends=('mkinitcpio'
         'mkinitcpio-nfs-utils'
         'bc')
install="${pkgname}.install"
source=('LICENSE'
        'whereisip.c'
        'hooks-multiflash'
        'install-multiflash'
        'mkinitcpio-multiflash.service'
        'rc.local.mkinitcpio-multiflash')
md5sums=('cefb772e5780b0fa3b7bbc09097e0ede'
         'b9afc4d03376a9184a09ee3b169aaff3'
         '84373e8da1e7ec819640d9590fecc5bb'
         'c2b88ab56fc7914a092f3709c164f212'
         'e8602c851f55b034bf4110c34347260b'
         '928bd9e1d2f1520602fcf4d3a9aa60e1')

build() {
  gcc -o whereisip -Os whereisip.c
}

package() {
  install -o root -g root -Dm755 whereisip "${pkgdir}/usr/bin/whereisip"
  install -o root -g root -Dm644 hooks-multiflash "${pkgdir}/usr/lib/initcpio/hooks/multiflash"
  install -o root -g root -Dm644 install-multiflash "${pkgdir}/usr/lib/initcpio/install/multiflash"
  install -o root -g root -Dm644 LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
  install -o root -g root -Dm644 mkinitcpio-multiflash.service "${pkgdir}/etc/systemd/system/mkinitcpio-multiflash.service"
  install -o root -g root -Dm755 rc.local.mkinitcpio-multiflash "${pkgdir}/etc/rc.local.mkinitcpio-multiflash"
}
