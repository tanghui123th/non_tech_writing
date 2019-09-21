function f() {
  this.if = function () {
    console.log(this === window);
  }
}