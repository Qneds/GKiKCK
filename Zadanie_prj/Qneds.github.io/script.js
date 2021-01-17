var scene = new THREE.Scene();
var aspect = window.innerWidth / window.innerHeight;
var camera = new THREE.PerspectiveCamera( 75, aspect, 0.1, 1000 );
var renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );

var loader_obj = new THREE.ObjectLoader();
var loader_tex = new THREE.TextureLoader();

var sled_y_off = 16;
var sled = new THREE.Object3D();


// wczytywanie sanek
loader_obj.load("sled_w_tex.json", function ( obj ) {
  obj.material.color.set("rgb(255, 255, 255)");
  obj.material.emissive.set(0x0);
  obj.material.specular.set("rgb(0, 0, 0)");
  obj.position.set(0,sled_y_off,0);
  obj.rotation.x = Math.PI / 12;
  scene.add( obj );
  sled = obj;
});

// wczytywanie drzewka
loader_obj.load("tree.json", function ( obj ) {
  obj.material = new THREE.MeshPhongMaterial({
    color : new THREE.Color("rgb(255, 255, 255)"),
    emissive : 0x0,
    emissiveIntensity : 1,
    specular : new THREE.Color("rgb(0, 0, 0)"),
    map : loader_tex.load("tree_tex.png")
  });
  obj.position.set(0,-3,6);
  obj.scale.set(2,2,2);
  obj.rotation.y = Math.PI / 6
  scene.add( obj );
});

// wczytywanie podłoża
loader_obj.load("plane.json", function ( obj ) {
  obj.material = new THREE.MeshPhongMaterial({
    color : new THREE.Color("rgb(255, 255, 255)"),
    emissive : 0x0,
    emissiveIntensity : 1,
    specular : new THREE.Color("rgb(0, 0, 0)"),
    map : loader_tex.load("plane_tex.jpg")
  });

  obj.position.set(0,-5,0);
  obj.scale.set(3,3,3);
  scene.add( obj );
});

// wczytywanie prezentu
loader_obj.load("gift.json", function ( obj ) {
  obj.material = new THREE.MeshPhongMaterial({
    color : new THREE.Color("rgb(255, 255, 255)"),
    emissive : 0x0,
    emissiveIntensity : 1,
    specular : new THREE.Color("rgb(0, 0, 0)"),
    map : loader_tex.load("gift_tex.png")
  });

  obj.position.set(0,-4,0);
  obj.scale.set(1.5,1.5,1.5);
  scene.add( obj );
});

// wczytywanie bałwana
loader_obj.load("snowman.json", function ( obj ) {
  obj.material = new THREE.MeshPhongMaterial({
    color : new THREE.Color("rgb(255, 255, 255)"),
    emissive : 0x0,
    emissiveIntensity : 1,
    specular : new THREE.Color("rgb(0, 0, 0)"),
    map : loader_tex.load("snowman_tex.png")
  });
  obj.position.set(0,-3.5,-6);
  obj.scale.set(2,2,2);
  obj.rotation.y = Math.PI / 6
  scene.add( obj );
});


// śwaitło 
var spot_light = new THREE.SpotLight(0xffffff, 0.5);
spot_light.position.set(-20, 50, 0);
spot_light.target.position.set(0,0,0);
spot_light.angle = Math.PI/10;
spot_light.penumbra = 0.5;
spot_light.castShadow = true;
scene.add(spot_light);

// obsługa kamery
camera.position.x = -30;
var controls = new THREE.OrbitControls(camera, renderer.domElement);
controls.minDistance = 20;
controls.maxDistance = 40;
controls.maxPolarAngle = 95* (Math.PI/180);
controls.enablePan = false;


var d = new Date();
var light_mode = 0;
var time_mem = d.getSeconds();
var render = function (x, y) {
  requestAnimationFrame( render );
  controls.update();

  // animacja sań
  d = new Date();
  sled.position.y = sled_y_off + Math.sin(d.getTime()/1000.0);
  
  // włączanie i wyłączanie światła
  if(light_mode == 0){
    var t = d.getSeconds();
    var time = t - time_mem
    if(time < 0)
      time = time + 60;
    if(time > 10){
      light_mode = 1;
      time_mem = d.getMilliseconds();
      spot_light.color.set(0);
    }
  } else if (light_mode == 1){
    var t = d.getMilliseconds();
    var time = t - time_mem
    if(time < 0)
      time = time + 1000;
    if(time > 250){
      light_mode = 2;
      time_mem = d.getMilliseconds();
      spot_light.color.set(0xffffff);
    }
  } else if (light_mode == 2){
    var t = d.getMilliseconds();
    var time = t - time_mem
    if(time < 0)
      time = time + 1000;
    if(time > 500){
      light_mode = 3;
      time_mem = d.getMilliseconds();
      spot_light.color.set(0);
    }
  } else if (light_mode == 3) {
    var t = d.getMilliseconds();
    var time = t - time_mem
    if(time < 0)
      time = time + 1000;
    if(time > 500){
      light_mode = 0;
      time_mem = d.getSeconds();
      spot_light.color.set(0xffffff);
    }
  }

  
  renderer.render( scene, camera );
};

render();