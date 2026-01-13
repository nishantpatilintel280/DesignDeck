(function () {
    const pf = document.getElementById('PlatformFilter');
    const sku = document.getElementById('SkuFilter');
    const form = document.getElementById('filterForm');
    if (pf) pf.addEventListener('change', () => form.submit());
    if (sku) sku.addEventListener('change', () => form.submit());

    document.querySelectorAll('.panel-toggle').forEach(btn => {
        const targetId = btn.getAttribute('data-bs-target');
        const target = document.querySelector(targetId);
        if (!target) return;
        target.addEventListener('show.bs.collapse', () => {
            btn.querySelector('i')?.classList.replace('bi-chevron-down', 'bi-chevron-up');
        });
        target.addEventListener('hide.bs.collapse', () => {
            btn.querySelector('i')?.classList.replace('bi-chevron-up', 'bi-chevron-down');
        });
    });

    const skuOptions = {
        PTL: ['12Xe', '4Xe', '404'],
        NVL: ['NVL-S', 'NVL-G', 'NVL-M'],
        LNL: ['ALL'],
        ARL: ['ALL']
    };
    const newPlat = document.getElementById('NewPlatform');
    const newSku = document.getElementById('NewSKU');

    // Read Model values from data attributes added to container
    const container = document.querySelector('.container-fluid.py-5');
    const modelPlatform = container?.getAttribute('data-model-platform') ?? '';
    const modelSku = container?.getAttribute('data-model-sku') ?? '';

    function fillNewSku(p) {
        if (!newSku) return;
        newSku.innerHTML = '<option value="" selected disabled>Select SKU</option>';
        (skuOptions[p] || ['ALL']).forEach(v => {
            const opt = document.createElement('option');
            opt.value = v; opt.textContent = v;
            newSku.appendChild(opt);
        });
        if (modelSku && modelSku !== 'ALL') {
            newSku.value = modelSku;
        } else {
            newSku.selectedIndex = 0; // keep "Select SKU" selected
        }
    }
    // Initialize with Model.Platform or current selection; always fill options
    if (newPlat) {
        const initialPlatform = (newPlat.value && newPlat.value !== '') ? newPlat.value : modelPlatform;
        if (initialPlatform && initialPlatform !== '') {
            newPlat.value = initialPlatform;
            fillNewSku(initialPlatform);
        } else {
            // fallback to PTL list so dropdown shows options
            fillNewSku('PTL');
            newSku.selectedIndex = 0;
        }
        newPlat.addEventListener('change', e => fillNewSku(e.target.value));
    }

    // Enable/disable Upload Excel button based on selected checkboxes
    var uploadLabel = document.getElementById('uploadExcelBtn');
    var uploadInput = document.getElementById('uploadExcelInput');
    var tooltipAnchor = document.getElementById('uploadExcelTooltipAnchor');
    var uploadTooltip = null;
    function ensureUploadTooltip() {
        if (tooltipAnchor && typeof bootstrap !== 'undefined' && bootstrap.Tooltip) {
            if (!uploadTooltip) {
                uploadTooltip = new bootstrap.Tooltip(tooltipAnchor);
            }
        }
    }
    function updateUploadEnableState() {
        if (!uploadLabel || !uploadInput) return;
        var anyChecked = !!document.querySelector('input[name="SelectedDesignIds"]:checked');
        if (anyChecked) {
            uploadLabel.classList.remove('disabled');
            uploadLabel.removeAttribute('aria-disabled');
            tooltipAnchor?.setAttribute('title', 'Upload Excel to update selected designs.');
            uploadInput.disabled = false;
        } else {
            uploadLabel.classList.add('disabled');
            uploadLabel.setAttribute('aria-disabled', 'true');
            tooltipAnchor?.setAttribute('title', 'Select designs to upload excel.');
            uploadInput.disabled = true;
        }
        // refresh tooltip to reflect updated title
        if (uploadTooltip && tooltipAnchor) {
            uploadTooltip.setContent && uploadTooltip.setContent({ '.tooltip-inner': tooltipAnchor.getAttribute('title') });
        }
    }
    // Observe changes on individual checkboxes and select-all
    function wireCheckboxEvents() {
        document.querySelectorAll('input[name="SelectedDesignIds"]').forEach(cb => {
            cb.addEventListener('change', updateUploadEnableState);
        });
        var selectAll = document.getElementById('selectAllDesigns');
        if (selectAll) {
            selectAll.addEventListener('change', function(){
                document.querySelectorAll('input[name="SelectedDesignIds"]').forEach(function(cb){ cb.checked = selectAll.checked; });
                updateUploadEnableState();
            });
        }
    }
    wireCheckboxEvents();
    ensureUploadTooltip();
    // Initial state on load
    updateUploadEnableState();

    window.hideFlashcard = function () {
        const overlay = document.getElementById('flashcardOverlay');
        const card = overlay?.querySelector('.flashcard');
        if (!overlay || !card) return;
        card.classList.remove('show');
        setTimeout(() => overlay.classList.remove('show'), 200);
    };
    window.showFlashcard = function () {
        const overlay = document.getElementById('flashcardOverlay');
        const card = overlay?.querySelector('.flashcard');
        if (!overlay || !card) return;
        overlay.classList.add('show');
        requestAnimationFrame(() => card.classList.add('show'));
        setTimeout(() => window.hideFlashcard(), 2000);
    };
    (function () {
        const overlay = document.getElementById('flashcardOverlay');
        const card = overlay?.querySelector('.flashcard');
        if (overlay?.classList.contains('show') && card?.classList.contains('show')) {
            setTimeout(() => window.hideFlashcard(), 2000);
        }
    })();
})();

document.addEventListener('DOMContentLoaded', function () {
  var modal = document.getElementById('addDesignModal');
  if (!modal) return;

  modal.addEventListener('shown.bs.modal', function () {
    try {
      var platform = document.querySelector('[data-model-platform]')?.getAttribute('data-model-platform');
      var sku = document.querySelector('[data-model-sku]')?.getAttribute('data-model-sku');

      var platformSelect = document.getElementById('NewPlatform');
      var skuSelect = document.getElementById('NewSKU');

      if (platformSelect && platform) {
        platformSelect.value = platform;
      }

      if (skuSelect && sku) {
        var hasOption = Array.from(skuSelect.options).some(function (opt) { return opt.value === sku; });
        if (hasOption) {
          skuSelect.value = sku;
        }
      }
    } catch (e) {
      console.warn('AddDesign modal init error:', e);
    }
  });
});