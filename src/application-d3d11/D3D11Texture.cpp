
namespace ml
{

void D3D11Texture::load(GraphicsDevice &g, const Bitmap &bmp)
{
    release(g);
    
    g.castD3D11().registerAsset(this);
    m_bmp = bmp;

    reset(g);
}

void D3D11Texture::release(GraphicsDevice &g)
{
    SAFE_RELEASE(m_texture);
    SAFE_RELEASE(m_view);
}

void D3D11Texture::reset(GraphicsDevice &g)
{
    release(g);

    auto &device = g.castD3D11().device();

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = (UINT)m_bmp.cols();
    desc.Height = (UINT)m_bmp.rows();
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    D3D_VALIDATE(device.CreateTexture2D(&desc, nullptr, &m_texture));
    D3D_VALIDATE(device.CreateShaderResourceView(m_texture, nullptr, &m_view));
}

void D3D11Texture::bind(GraphicsDevice &g) const
{
    auto &context = g.castD3D11().context();
    context.PSSetShaderResources(0, 1, &m_view);
}

}