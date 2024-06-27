document.addEventListener('DOMContentLoaded', () => {
    const cube = document.querySelector('.cube');
    let isDragging = false;
    let startX, startY;
    let currentX = 0, currentY = 0;

    cube.addEventListener('mousedown', (e) => {
        isDragging = true;
        startX = e.clientX;
        startY = e.clientY;
    });

    document.addEventListener('mousemove', (e) => {
        if (isDragging) {
            const deltaX = e.clientX - startX;
            const deltaY = e.clientY - startY;
            currentX += deltaX * 0.9;
            currentY -= deltaY * 0.9;
            cube.style.transform = `rotateX(${currentY}deg) rotateY(${currentX}deg)`;
            startX = e.clientX;
            startY = e.clientY;
        }
    });

    document.addEventListener('mouseup', () => {
        isDragging = false;
    });

    cube.addEventListener('mouseleave', () => {
        isDragging = false;
    });
});
