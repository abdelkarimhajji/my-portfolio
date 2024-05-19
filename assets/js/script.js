'use strict';



// element toggle function
const elementToggleFunc = function (elem) { elem.classList.toggle("active"); }



// sidebar variables
const sidebar = document.querySelector("[data-sidebar]");
const sidebarBtn = document.querySelector("[data-sidebar-btn]");

// sidebar toggle functionality for mobile
sidebarBtn.addEventListener("click", function () { elementToggleFunc(sidebar); });



// testimonials variables
const testimonialsItem = document.querySelectorAll("[data-testimonials-item]");
const modalContainer = document.querySelector("[data-modal-container]");
const modalCloseBtn = document.querySelector("[data-modal-close-btn]");
const overlay = document.querySelector("[data-overlay]");

// modal variable
const modalImg = document.querySelector("[data-modal-img]");
const modalTitle = document.querySelector("[data-modal-title]");
const modalText = document.querySelector("[data-modal-text]");

// modal toggle function
const testimonialsModalFunc = function () {
  modalContainer.classList.toggle("active");
  overlay.classList.toggle("active");
}

// add click event to all modal items
for (let i = 0; i < testimonialsItem.length; i++) {

  testimonialsItem[i].addEventListener("click", function () {

    modalImg.src = this.querySelector("[data-testimonials-avatar]").src;
    modalImg.alt = this.querySelector("[data-testimonials-avatar]").alt;
    modalTitle.innerHTML = this.querySelector("[data-testimonials-title]").innerHTML;
    modalText.innerHTML = this.querySelector("[data-testimonials-text]").innerHTML;

    testimonialsModalFunc();

  });

}

// add click event to modal close button
modalCloseBtn.addEventListener("click", testimonialsModalFunc);
overlay.addEventListener("click", testimonialsModalFunc);



// custom select variables
const select = document.querySelector("[data-select]");
const selectItems = document.querySelectorAll("[data-select-item]");
const selectValue = document.querySelector("[data-selecct-value]");
const filterBtn = document.querySelectorAll("[data-filter-btn]");

select.addEventListener("click", function () { elementToggleFunc(this); });

// add event in all select items
for (let i = 0; i < selectItems.length; i++) {
  selectItems[i].addEventListener("click", function () {

    let selectedValue = this.innerText.toLowerCase();
    selectValue.innerText = this.innerText;
    elementToggleFunc(select);
    filterFunc(selectedValue);

  });
}

// filter variables
const filterItems = document.querySelectorAll("[data-filter-item]");

const filterFunc = function (selectedValue) {

  for (let i = 0; i < filterItems.length; i++) {

    if (selectedValue === "all") {
      filterItems[i].classList.add("active");
    } else if (selectedValue === filterItems[i].dataset.category) {
      filterItems[i].classList.add("active");
    } else {
      filterItems[i].classList.remove("active");
    }

  }

}

// add event in all filter button items for large screen
let lastClickedBtn = filterBtn[0];

for (let i = 0; i < filterBtn.length; i++) {

  filterBtn[i].addEventListener("click", function () {

    let selectedValue = this.innerText.toLowerCase();
    selectValue.innerText = this.innerText;
    filterFunc(selectedValue);

    lastClickedBtn.classList.remove("active");
    this.classList.add("active");
    lastClickedBtn = this;

  });

}



// contact form variables
const form = document.querySelector("[data-form]");
const formInputs = document.querySelectorAll("[data-form-input]");
const formBtn = document.querySelector("[data-form-btn]");

// add event to all form input field
for (let i = 0; i < formInputs.length; i++) {
  formInputs[i].addEventListener("input", function () {

    // check form validation
    if (form.checkValidity()) {
      formBtn.removeAttribute("disabled");
    } else {
      formBtn.setAttribute("disabled", "");
    }

  });
}



// page navigation variables
const navigationLinks = document.querySelectorAll("[data-nav-link]");
const pages = document.querySelectorAll("[data-page]");

// add event to all nav link
for (let i = 0; i < navigationLinks.length; i++) {
  navigationLinks[i].addEventListener("click", function () {

    for (let i = 0; i < pages.length; i++) {
      if (this.innerHTML.toLowerCase() === pages[i].dataset.page) {
        pages[i].classList.add("active");
        navigationLinks[i].classList.add("active");
        window.scrollTo(0, 0);
      } else {
        pages[i].classList.remove("active");
        navigationLinks[i].classList.remove("active");
      }
    }

  });
}

let projectItem = document.getElementsByClassName("project-item");
let containerDetails = document.getElementsByClassName("containerDetails");
let valid = false;
function removeClass() {
  for (let i = 0; i < projectItem.length; i++) {
    projectItem[i].classList.remove("expand");
    projectItem[i].firstElementChild.classList.remove("allElements");
  };

}
function removeStyle() {
  for (let i = 0; i < containerDetails.length; i++) {
    if(containerDetails[i].style.display == "block") {
      // alert("hello")
      containerDetails[i].style.display = "none";
    }
  };
}

function removeClassDeatils()
{
  event.stopPropagation();
  removeStyle();
  removeClass();
}
for (let i = 0; i < projectItem.length; i++) {

  projectItem[i].addEventListener("click", function () {
    if(!projectItem[i].classList.contains("expand") && i != projectItem.length - 1){
    
      removeStyle();
      removeClass();
        projectItem[i].classList.add("expand");
        projectItem[i].firstElementChild.setAttribute("class", "allElements");
        containerDetails[i].style.display = "block";
        if(window.scrollY >= 0) {
          // window.scrollTo({ top: 0, behavior: 'smooth' });
          let element = document.querySelector('.expand');
          let position = element.getBoundingClientRect();
          console.log(position);
          console.log('Scroll position:',  window.scrollY);

          window.scrollTo({ top: window.scrollY + position.top - 20, behavior: 'smooth' })
          // alert(position.top);
          // window.scrollTo(0, position.top);
        }
    }
  });
}
function handleClick(event) {
  // console.log("calses ; ",event.target.className);
  console.log("Parent class: ",event.target.parentNode.className+"/")
  let classes = ["project-item", "project-img","project-item-icon-box"
  ,"myImg", "allElements", "hydrated", "navbar-item", "containerDetails",
  "container-desc","container-image","containerTitle",
  "container-all","desc","container-badges","container-links","myspan",
  "myTitles"


  ]

  if(!classes.includes(event.target.className))
  {
    removeClassDeatils();
    removeClass();
  }
  
}

window.addEventListener('click', handleClick);

let cursure = document.getElementById("cursore");
let jsHover = document.getElementsByClassName("jsHover");
document.querySelector('html').addEventListener('mousemove', function(e) {
  var x = e.pageX - about.offsetLeft+25;
  var y = e.pageY - about.offsetTop -0;
  cursure.style.left = x + "px";
  cursure.style.top = y + "px";
  // console.log('Mouse position: ', x, y);
});
let isCursorInWindow = false;
window.addEventListener('mousemove', function() {
  isCursorInWindow = true;
  cursure.style.display = "block";
});

window.addEventListener('mouseout', function() {
  isCursorInWindow = false;
});
setInterval(function() {
  if (!isCursorInWindow) {
    cursure.style.display = "none";
  }
}, 1000);

