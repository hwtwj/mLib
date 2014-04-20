namespace ml {

namespace shapes {

float cubeVData[8][3] = {
  {1.0f, 1.0f, 1.0f}, { -1.0f, 1.0f, 1.0f}, { -1.0f, -1.0f, 1.0f},
  {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, -1.0f}, { -1.0f, 1.0f, -1.0f},
  { -1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}
};

int cubeIData[12][3] = {
  {1, 2, 3}, {1, 3, 0}, {0, 3, 7}, {0, 7, 4}, {3, 2, 6},
  {3, 6, 7}, {1, 6, 2}, {1, 5, 6}, {0, 5, 1}, {0, 4, 5},
  {6, 5, 4}, {6, 4, 7}
};

int cubeEData[12][2] = {
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

TriMeshOld box(float xDim, float yDim, float zDim, const vec4f& color) {
  std::vector<TriMeshOld::TriMeshOldVertex> vv(8);
  std::vector<UINT> vi(12 * 3);

  // Vertices
  for (int i = 0; i < 8; i++) {
    vv[i].position = vec3f(cubeVData[i][0], cubeVData[i][1], cubeVData[i][2]);
    vv[i].normal = vec3f(1.0f, 0.0f, 0.0f);  // TODO(ms): write and call generateNormals() function
    vv[i].attributeA = color;
  }

  // Triangles
  for (int i = 0; i < 12; i++) {
    vi[i * 3 + 0] = cubeIData[i][0];
    vi[i * 3 + 1] = cubeIData[i][1];
    vi[i * 3 + 2] = cubeIData[i][2];
  }

  TriMeshOld mesh(vv, vi);
  mesh.stretch(vec3f(0.5f * xDim, 0.5f * yDim, 0.5f * zDim));

  return mesh;
}

TriMeshOld cylinder(float radius, float height, UINT stacks, UINT slices, const vec4f& color) {
  std::vector<TriMeshOld::TriMeshOldVertex> vertices((stacks + 1) * slices);
  std::vector<UINT> indices(stacks * slices * 6);

  UINT vIndex = 0;
  for (UINT i = 0; i <= stacks; i++)
    for (UINT i2 = 0; i2 < slices; i2++) {
      auto& vtx = vertices[vIndex++];
      float theta = float(i2) * 2.0f * math::PIf / float(slices);
      vtx.position = vec3f(radius * cosf(theta), radius * sinf(theta), height * float(i) / float(stacks));
      vtx.normal = vec3f(1.0f, 0.0f, 0.0f);  // TODO(ms): write and call generateNormals() function
      vtx.attributeA = color;
    }

  UINT iIndex = 0;
  for (UINT i = 0; i < stacks; i++) {
    for (UINT i2 = 0; i2 < slices; i2++) {
      int i2p1 = (i2 + 1) % slices;

      indices[iIndex++] = (i + 1) * slices + i2;
      indices[iIndex++] = i * slices + i2;
      indices[iIndex++] = i * slices + i2p1;


      indices[iIndex++] = (i + 1) * slices + i2;
      indices[iIndex++] = i * slices + i2p1;
      indices[iIndex++] = (i + 1) * slices + i2p1;
    }
  }

  return TriMeshOld(vertices, indices);
}

TriMeshOld cylinder(const vec3f& p0, const vec3f& p1, float radius, UINT stacks, UINT slices, const vec4f& color) {
  float height = (p1 - p0).length();

  TriMeshOld result = shapes::cylinder(radius, height, stacks, slices, color);
  result.applyTransform(mat4f::translation(p0) * mat4f::face(vec3f::eZ, p1 - p0));
  return result;
}

TriMeshOld wireframeBox(float dim, const vec4f& color) {
  std::vector<ml::TriMeshOld> meshes;
  ml::vec3f v[8];  std::memmove(v, cubeVData, sizeof(v[0]) * 8);
  for (uint i = 0; i < 12; i++) {
    meshes.push_back(line(dim * v[cubeEData[i][0]], dim * v[cubeEData[i][1]], color));
  }
  return meshutil::createUnifiedMesh(meshes);
}

TriMeshOld wireframeBox(const mat4f& xf, const vec4f& color) {
  std::vector<ml::TriMeshOld> meshes;
  ml::vec3f v[8];  std::memmove(v, cubeVData, sizeof(v[0]) * 8);
  for (uint i = 0; i < 8; i++) { v[i] = xf * v[i]; }
  for (unsigned int i = 0; i < 12; i++) {
    const ml::vec3f& p0 = v[cubeEData[i][0]];
    const ml::vec3f& p1 = v[cubeEData[i][1]];
    meshes.push_back(line(p0, p1, color));
  }
  return meshutil::createUnifiedMesh(meshes);
}

}  // namespace shapes

}  // namespace ml