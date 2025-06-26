function typewriterEffect(element, text, speed = 120) {
  let i = 0;
  function type() {
    if (i <= text.length) {
      element.innerHTML = text.slice(0, i) + '<span class="blink-caret"></span>';
      i++;
      setTimeout(type, speed);
    }
  }
  type();
}

document.addEventListener("DOMContentLoaded", function () {
  const elements = document.querySelectorAll(".typing-container");

  elements.forEach((el) => {
    const text = el.getAttribute("data-text") || "";
    const speed = parseInt(el.getAttribute("data-speed") || "120");
    typewriterEffect(el, text, speed);
  });
});
