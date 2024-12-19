import Zooming from 'zooming'

new Zooming().listen('img')

document.addEventListener('DOMContentLoaded', function () {
    const zooming = new Zooming({
      // options...
    })
    zooming.listen('img')
  })
  