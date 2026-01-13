const dropdownIds = ['Platform', 'OEM', 'Design_Name', 'SKU'];
const fileInputs = ['EDID_file', 'DPCD_file', 'VBT_file'];
const warningMsg = document.getElementById('dropdownWarning');

function checkDropdowns() {
    // Check if all dropdowns have a selected value
    let allSelected = dropdownIds.every(id => {
        const element = document.getElementById(id);
        return element && !!element.value;
    });

    // Enable or disable file inputs accordingly
    fileInputs.forEach(id => {
        const el = document.getElementById(id);
        if (el) {
            el.disabled = !allSelected;
            if (!allSelected) {
                el.value = '';  // clear file input if disabling
            }
        }
    });

    // Show warning if dropdowns NOT all selected
    if (warningMsg) {
        if (!allSelected) {
            warningMsg.style.display = 'block';
        } else {
            warningMsg.style.display = 'none';
        }
    }
}

// Attach change event listeners to dropdowns to check on every change
dropdownIds.forEach(id => {
    const element = document.getElementById(id);
    if (element) {
        element.addEventListener('change', () => {
            checkDropdowns();
        });
    }
});

// Initial check on page load
checkDropdowns();

// Bootstrap validation on form submit
const detailsForm = document.getElementById('detailsForm');
if (detailsForm) {
    detailsForm.addEventListener('submit', function (event) {
        const form = this;
        if (!form.checkValidity()) {
            event.preventDefault();
            event.stopPropagation();
        }
        form.classList.add('was-validated');
    }, false);
}

document.addEventListener('DOMContentLoaded', function () {
    const closeAllAccordions = () => {
        // Get all accordion collapses
        document.querySelectorAll('.accordion-collapse').forEach(collapse => {
            // Force close using Bootstrap's Collapse API
            const bsCollapse = bootstrap.Collapse.getInstance(collapse);
            if (bsCollapse) {
                bsCollapse.hide();
            }

            // Manually reset classes and styles
            collapse.classList.remove('show', 'collapsing');
            collapse.style.height = '';
            collapse.setAttribute('aria-expanded', 'false');
        });

        // Reset all accordion buttons
        document.querySelectorAll('.accordion-button').forEach(button => {
            button.classList.add('collapsed');
            button.setAttribute('aria-expanded', 'false');
        });
    };

    // Close accordions when switching tabs
    document.addEventListener('show.bs.tab', function (event) {
        // Small delay to ensure tab content is ready
        setTimeout(() => {
            closeAllAccordions();
        }, 50);
    });

    // Also close when tab is fully shown
    document.addEventListener('shown.bs.tab', function (event) {
        closeAllAccordions();
    });

    // Close accordions when clicking on panel list items
    document.querySelectorAll('.list-group-item[data-bs-toggle="tab"]').forEach(item => {
        item.addEventListener('click', function () {
            setTimeout(() => {
                closeAllAccordions();
            }, 100);
        });
    });

    // Initial state - close all accordions
    setTimeout(() => {
        closeAllAccordions();
    }, 100);

    // Enable Save button on edits
    const specsForm = document.getElementById('specsForm');
    const saveBtn = document.getElementById('saveSpecsBtn');
    if (specsForm && saveBtn) {
        saveBtn.disabled = true;
        specsForm.querySelectorAll('input, select, textarea').forEach(el => {
            el.addEventListener('input', () => saveBtn.disabled = false);
            el.addEventListener('change', () => saveBtn.disabled = false);
        });
    }
});