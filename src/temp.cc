#include <GL/glew.h>
#include <GL/glut.h>

Mesh mesh;  // Ваш экземпляр класса Mesh с данными

GLuint vertexBuffer, indexBuffer;

void initBuffers() {
    // Инициализация буфера вершин
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    // Инициализация буфера индексов для поверхностей
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    // Рассматриваем каждую поверхность и добавляем её индексы в буфер
    std::vector<int> allIndices;
    for (const Face& face : mesh.faces) {
        allIndices.insert(allIndices.end(), face.vertexIndices.begin(), face.vertexIndices.end());
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(int), &allIndices[0], GL_STATIC_DRAW);
}

void drawPoints() {
    glPointSize(5.0f);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    glDrawArrays(GL_POINTS, 0, mesh.vertices.size());

    glDisableVertexAttribArray(0);
}

void drawMesh() {
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Рисование точек
    drawPoints();

    // Рисование поверхностей
    drawMesh();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("OpenGL Example");

    glEnable(GL_DEPTH_TEST);
    glewInit();

    // Загрузка данных в буферы
    initBuffers();

    glutDisplayFunc(display);
    glutMainLoop();

    // Освобождение ресурсов OpenGL
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);

    return 0;
}
