#include "GraphicsPch.h"
#include "ASEParser/CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(GeomObject* pGeomObj)
{
	// �ٽ� �־��.. x, y���� �����ϰ�. z��� ����.
	XMVECTOR _cross = XMVector3Cross(pGeomObj->m_tm_row0, pGeomObj->m_tm_row2);
	if (XMVector3Dot(_cross, pGeomObj->m_tm_row1).m128_f32[0] < 0)
	{
		pGeomObj->m_tm_row0 *= -1;
		pGeomObj->m_tm_row1 *= -1;
		pGeomObj->m_tm_row2 *= -1;

		for (auto& value : pGeomObj->m_meshface)
		{
			value->m_normal *= -1;
			value->m_normalvertex[0] *= -1;
			value->m_normalvertex[1] *= -1;
			value->m_normalvertex[2] *= -1;
		}
	}

	// ��Ű�� �� ����..!
	if (pGeomObj->m_is_skinningobject)
	{
		// �� ���ؽ��� ��Ű�� ���� ������ �־��ش�?
		// WVertex��.. �ɰ����� ���ؽ��� ����� ���� �ε����� ����ġ���� �� �ִ�..
	}

	// opt ���ؽ��� �ϴ� �� �־��ְ� Ư�� ��Ȳ���� �ɰ��ش�.
		// ���� �ѰŴ� �� �ɰ� �� �־��� �ſ� ������.
	// ���ؽ� �ϴ� ����
	for (int i = 0; i < pGeomObj->m_meshvertex.size(); i++)
	{
		// �� ���ؽ��� ���� �־��ش�.
		Vertex* _pVertex = new Vertex;

		// �Ľ��� ��ġ���� �״�� �־��ش�.
		_pVertex->m_pos = pGeomObj->m_meshvertex[i]->m_pos;

		// ���� ������ �־��ش�.
		_pVertex->m_normal = pGeomObj->m_meshvertex[i]->m_normal;
		// �⺻������ ����� �븻�� ���õǾ� ���� �ʴ�.
		_pVertex->m_isnormalset = pGeomObj->m_meshvertex[i]->m_isnormalset;

		// �ؽ��� ��ǥ�� �־��ش�.
		_pVertex->u = pGeomObj->m_meshvertex[i]->u;
		_pVertex->v = pGeomObj->m_meshvertex[i]->v;
		_pVertex->m_istextureset = pGeomObj->m_meshvertex[i]->m_istextureset;

		// ��Ų�带 ���� ���� �������� �־��ش�.
		_pVertex->m_bw1 = pGeomObj->m_meshvertex[i]->m_bw1;
		_pVertex->m_bw2 = pGeomObj->m_meshvertex[i]->m_bw2;
		_pVertex->m_bw3 = pGeomObj->m_meshvertex[i]->m_bw3;

		// �ε��� ������ �־��ش�. => �ش� �ε��� ��ȣ�� Wvertex�� �������� �� ���� �� �ϴ�.
		_pVertex->m_indices = pGeomObj->m_meshvertex[i]->m_indices;

		// �׸��� �ϴ� �ش� �������� �־��ش�.
		pGeomObj->m_opt_vertex.push_back(_pVertex);
	}

	// face�� ���鼭 ��ְ��� �������ش�.
		// �� �κ��� ������, �����ǰ� �븻�� ���õǾ� �ִ� ģ���� ���´�.
	for (int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		// �� ���̽� �� �ε��� �� ���� ���鼭.. �븻���� ���õǾ� ���� ������ �������ش�.
		for (int j = 0; j < 3; j++)
		{
			// ���ؽ� �ϳ��ϳ��� ���鼭 opt_vertex�� ������ �������ش�.
				// i��° ���� j��° �ε����� ���ؽ� ���� �����´�.
			Vertex* _nowVertex = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]];

			// �븻���� ���õǾ� �ִ°�.
				// �⺻���� false�� ������ ���⼭ �������ش�.
			if (!_nowVertex->m_isnormalset)
			{
				// normal face�� �߰����� normal ���� �־��ݴϴ�.
				_nowVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
				// �׸��� ���� ���ؽ��� �븻���� ���õǾ��ٰ� ���ݴϴ�.
				_nowVertex->m_isnormalset = true;
			}
			// ������ �Ǿ����� ��, ���� �߰� ���ش� = convert all�� �������
				// �븻 ���� ���ؼ� �ٸ��� �߰����ش�.
			else
			{
				// �븻 ���� �ѹ��� �����Ǿ� �ִ�.
					// ������ �븻 ���� �ٸ� ���� �����Ѵ�.
				if (_nowVertex->m_normal != pGeomObj->m_meshface[i]->m_normalvertex[j])
				{
					// �븻 ���� �ٸ��� �� ���ؽ��� ���� �־�����.
					Vertex* _newVertex = new Vertex();

					// �������� ������ �� �� �ִ�.
					int vertexIndex = pGeomObj->m_meshface[i]->m_vertexindex[j];
					_newVertex->m_pos = pGeomObj->m_opt_vertex[vertexIndex]->m_pos;

					// �� �븻 ���� �־��ݴϴ�.
					_newVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
					// �븻 ���� �־���⿡ ���õǾ��ٰ� ���ݴϴ�.
					_newVertex->m_isnormalset = true;

					// ���ؽ��� �ɰ� �� �� ���� �ε��� ��ȣ�� ����մϴ�. (�������� ���� ��ġ�� �ִ� ���ؽ��� ����)
						// �׸� �׸� �� ����ϴ� �ε����� �ٸ� �ε����Դϴ�.
					_newVertex->m_indices = _nowVertex->m_indices;

					// �� ���ؽ��� �־��ݴϴ�.
					pGeomObj->m_opt_vertex.push_back(_newVertex);

					// ���̽��� index���鵵 �������ݴϴ�.
					pGeomObj->m_meshface[i]->m_vertexindex[j] = pGeomObj->m_opt_vertex.size() - 1;
				}
			}
		}
	}

	// �븻���� split�� �߱� ������.. ��� ���� ���鼭 Texture ���� �־��.
	for (int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// ���ؽ� �ϳ��ϳ��� ���鼭 opt_vertex�� ������ �������ش�.
			Vertex* _nowVertex = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]];

			// �ؽ��� ���� ������ ���� ������ �������� �ʿ䰡 ���� ������ break;
			if (pGeomObj->m_mesh_numtvertex == 0)
				break;

			// �ؽ��� ���� �����Ǿ� �ִ°�.
			if (!_nowVertex->m_istextureset)
			{
				// �ش� ���̽��� �ε��� ���� �����´�..
				_nowVertex->u = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_u;
				_nowVertex->v = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_v;

				_nowVertex->m_istextureset = true;
			}
			// ������ �Ǿ����� ��..
				// �ؽ��� ���� �ٸ� ���� �����Ѵ�.
				// �� ���� �븻���� ���..
			else
			{
				if ((_nowVertex->u != pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_u)
					||
					(_nowVertex->v != pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_v))
				{
					// �� ������ �븻 ���� �����´�..?
					Vertex* _newVertex = new Vertex();

					// ������, �븻�� ���ٰ� ����.
					_newVertex->m_pos = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]]->m_pos;
					_newVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
					_newVertex->m_isnormalset = true;

					// �ؽ��ĸ� �־��ݴϴ�.
					_newVertex->u = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_u;
					_newVertex->v = pGeomObj->m_mesh_tvertex[pGeomObj->m_meshface[i]->m_TFace[j]]->m_v;

					// ���ؽ��� �ɰ� �� �� ���� �ε��� ��ȣ�� ����մϴ�. (�������� ���� ��ġ�� �ִ� ���ؽ��� ����)
						// �׸� �׸� �� ����ϴ� �ε����� �ٸ� �ε����Դϴ�.
					_newVertex->m_indices = _nowVertex->m_indices;

					// �� ���ؽ��� �ְ� �ε��� ���� �������ݴϴ�.
					pGeomObj->m_opt_vertex.push_back(_newVertex);
					pGeomObj->m_meshface[i]->m_vertexindex[j] = pGeomObj->m_opt_vertex.size() - 1;
				}
			}
		}
	}

	// �ϴ� ���⼭ opt_index�� �� �� ���������(position�� ������ �ִ� index ����)
	pGeomObj->m_opt_index = new IndexList[pGeomObj->m_meshface.size()];

	for (unsigned int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pGeomObj->m_opt_index[i].index[j] = pGeomObj->m_meshface[i]->m_vertexindex[j];
		}
	}
	// ������� ���� ����
	return true;
}

bool CASEParser::Convert_OptimizeLight(GeomObject* pGeomObj)
{
	// �ٽ� �־��.. x, y���� �����ϰ�. z��� ����. => �Լ� ����� ���ô�..
	XMVECTOR _cross = XMVector3Cross(pGeomObj->m_tm_row0, pGeomObj->m_tm_row2);
	if (XMVector3Dot(_cross, pGeomObj->m_tm_row1).m128_f32[0] < 0)
	{
		pGeomObj->m_tm_row0 *= -1;
		pGeomObj->m_tm_row1 *= -1;
		pGeomObj->m_tm_row2 *= -1;

		for (auto& value : pGeomObj->m_meshface)
		{
			value->m_normal *= -1;
			value->m_normalvertex[0] *= -1;
			value->m_normalvertex[1] *= -1;
			value->m_normalvertex[2] *= -1;
		}
	}

	// opt ���ؽ��� �ϴ� �� �־��ְ� Ư�� ��Ȳ���� �ɰ��ش�.
		// ���� �ѰŴ� �� �ɰ� �� �־��� �ſ� ������.
	// ���ؽ� �ϴ� ����
	for (int i = 0; i < pGeomObj->m_meshvertex.size(); i++)
	{
		// �� ���ؽ��� ���� �־��ش�.
		Vertex* _pVertex = new Vertex;

		// �Ľ��� ��ġ���� �״�� �־��ش�.
		_pVertex->m_pos = pGeomObj->m_meshvertex[i]->m_pos;

		// ���� ������ �־��ش�.
		_pVertex->m_normal = pGeomObj->m_meshvertex[i]->m_normal;
		// �⺻������ ����� �븻�� ���õǾ� ���� �ʴ�.
		_pVertex->m_isnormalset = pGeomObj->m_meshvertex[i]->m_isnormalset;

		// �ؽ��� ��ǥ�� �־��ش�.
		_pVertex->u = pGeomObj->m_meshvertex[i]->u;
		_pVertex->v = pGeomObj->m_meshvertex[i]->v;
		_pVertex->m_istextureset = pGeomObj->m_meshvertex[i]->m_istextureset;

		// ��Ų�带 ���� ���� �������� �־��ش�.
		_pVertex->m_bw1 = pGeomObj->m_meshvertex[i]->m_bw1;
		_pVertex->m_bw2 = pGeomObj->m_meshvertex[i]->m_bw2;
		_pVertex->m_bw3 = pGeomObj->m_meshvertex[i]->m_bw3;

		// �ε��� ������ �־��ش�.
		_pVertex->m_indices = pGeomObj->m_meshvertex[i]->m_indices;

		// �׸��� �ϴ� �ش� �������� �־��ش�.
		pGeomObj->m_opt_vertex.push_back(_pVertex);
	}

	// face�� ���鼭 ��ְ��� �������ش�.
		// �� �κ��� ������, �����ǰ� �븻�� ���õǾ� �ִ� ģ���� ���´�.
	for (int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		// �� ���̽� �� �ε��� �� ���� ���鼭.. �븻���� ���õǾ� ���� ������ �������ش�.
		for (int j = 0; j < 3; j++)
		{
			// ���ؽ� �ϳ��ϳ��� ���鼭 opt_vertex�� ������ �������ش�.
				// i��° ���� j��° �ε����� ���ؽ� ���� �����´�.
			Vertex* _nowVertex = pGeomObj->m_opt_vertex[pGeomObj->m_meshface[i]->m_vertexindex[j]];

			// �븻���� ���õǾ� �ִ°�.
				// �⺻���� false�� ������ ���⼭ �������ش�.
			if (!_nowVertex->m_isnormalset)
			{
				// normal face�� �߰����� normal ���� �־��ݴϴ�.
				_nowVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
				// �׸��� ���� ���ؽ��� �븻���� ���õǾ��ٰ� ���ݴϴ�.
				_nowVertex->m_isnormalset = true;
				// �ؽ��İ� ���� ������ ���ؽ��� ������ �ؽ��� ���� �־��ݴϴ�.
				_nowVertex->u = 0.0f;
				_nowVertex->v = 0.0f;
				_nowVertex->m_istextureset = true;

			}
			// ������ �Ǿ����� ��, ���� �߰� ���ش� = convert all�� �������
				// �븻 ���� ���ؼ� �ٸ��� �߰����ش�.
			else
			{
				// �븻 ���� �ѹ��� �����Ǿ� �ִ�.
					// ������ �븻 ���� �ٸ� ���� �����Ѵ�.
				if (_nowVertex->m_normal != pGeomObj->m_meshface[i]->m_normalvertex[j])
				{
					// �븻 ���� �ٸ��� �� ���ؽ��� ���� �־�����.
					Vertex* _newVertex = new Vertex();

					// �������� ������ �� �� �ִ�.
					int vertexIndex = pGeomObj->m_meshface[i]->m_vertexindex[j];
					_newVertex->m_pos = pGeomObj->m_opt_vertex[vertexIndex]->m_pos;

					// �� �븻 ���� �־��ݴϴ�.
					_newVertex->m_normal = pGeomObj->m_meshface[i]->m_normalvertex[j];
					// �븻 ���� �־���⿡ ���õǾ��ٰ� ���ݴϴ�.
					_newVertex->m_isnormalset = true;

					_newVertex->u = 0.0f;
					_newVertex->v = 0.0f;
					_newVertex->m_istextureset = true;

					// �� ���ؽ��� �־��ݴϴ�.
					pGeomObj->m_opt_vertex.push_back(_newVertex);

					// �ε��� ������ �������ݴϴ�.
					//pMesh->m_opt_index[i].index[j] = pMesh->m_opt_vertex.size() - 1;

					// ���̽��� index���鵵 �������ݴϴ�.
					pGeomObj->m_meshface[i]->m_vertexindex[j] = pGeomObj->m_opt_vertex.size() - 1;
				}
			}
		}
	}

	// �ϴ� ���⼭ opt_index�� �� �� ���������(position�� ������ �ִ� index ����)
	pGeomObj->m_opt_index = new IndexList[pGeomObj->m_meshface.size()];

	for (unsigned int i = 0; i < pGeomObj->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pGeomObj->m_opt_index[i].index[j] = pGeomObj->m_meshface[i]->m_vertexindex[j];
		}
	}
	// ������� ���� ����
	return true;
}

// ���� ���� ConvertAll
	// �ߺ� �Ű� �Ⱦ��� �ϴ� �� �ɰ��ִ� �Լ�..
bool CASEParser::ConvertAll(GeomObject* pMesh)
{
	// �켱 Ư�� ��ġ�� ���ؽ��� �ӽ÷� ���� => �ش� ������ �������� ����Ѵ�.
	std::vector<Vertex*> _posVertex;

	for (int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		_posVertex.push_back(_pVertex);
	}

	// Light������..
		// ���Ŀ� �ؽ��Ĵ� �������� ������ ���� ��� �ذ� �� �� ������..
	// ���� �ε����� �׷����� ������� �� �ִ�..
		// �븻�� ��� ABC�� �ƴ� ACB�� �� ���� ��.. => �״�� �־��� ����
		// �� �κ��� �ٲ��ְ� �ε����� ������� �־��ش�.
		// ���ؽ��� �븻 ���� �־��ְ� �ε������� �ε��� ī��Ʈ�� ������� �־��ش�..
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	// �������� face�� �������� �� �ε����� ã�ƾ� ��
		// ���̽� ���� * 3���� ���鼭 �߰����ش�.

	/*unsigned int _faceCount = pMesh->m_meshface.size();
	TRACE("FaceCount : %d", _faceCount);*/

	int indexCount = 0;

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		//TRACE("FaceNum : %d / %d\n", i, _faceCount);
		for (int j = 0; j < 3; j++)
		{
			// �� ���ؽ��� ���� opt_vertex�� �־��ش�.
			Vertex* _pVertex = new Vertex;
			// �������� �ش� �ε����� �ִ� ���ؽ��� ������.
			_pVertex->m_pos = _posVertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_pos;

			// �븻 ����, �ش� �޽� ���̽��� �ش��ϴ� ���ؽ��� �븻��.
				// �ε� j�� 1�̸� 2�� ����, 2�� 1�� ���� �־��ش�.
			_pVertex->m_normal = pMesh->m_meshface[i]->m_normalvertex[j];

			// ���� ���� ���ؽ��� �־��ְ�..
			pMesh->m_opt_vertex.push_back(_pVertex);

			// �ε���ī��Ʈ�� �ø��鼭 �־��ش�.
				// ���θ��� ���ؽ����� �ߺ����� 0, 1, 2, 3, 4, 5 ���� ������ ���� �� ��.
			pMesh->m_opt_index[i].index[j] = indexCount++;
		}
	}


	// ������ �ӽ� ��ü�� �޸𸮸� ������ �ش�
	for (auto i : _posVertex)
	{
		delete i;
	}

	return true;
}


ASEParser::GeomObject* CASEParser::GetGeomObj(int index)
{
	return m_GeomObjList[index];
}

//----------------------------------------------------------------
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	// �븻 ���� ����.. ���̽� ����(�� �κ� �� �� ���ƾ� �� �� ������..)
	int _tempFace = 0;


	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

		static int iv = 0;

		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:

			Parsing_DivergeRecursiveALL(depth++);
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------

		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberLong();
			break;

			//--------------------
			// COMMENT
			//--------------------

		case TOKENR_COMMENT:
			Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
			break;

			//--------------------
			// SCENE
			//--------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_firstframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_lastframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_framespeed = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_ticksperframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			m_scenedata.m_meshframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			m_scenedata.m_keyframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
			break;

		case TOKENR_SCENE_ENVMAP:
		{
			// �׳� ���� ������ �о������ }�� ���ö�����
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;

		//--------------------
		// MATERIAL_LIST
		//--------------------

		case TOKENR_MATERIAL_COUNT:
		{
			m_materialcount = Parsing_NumberInt();
		}
		break;

		// ���׸����� ������ �� ���׸����� �ϳ� ���� ���׸��� ����Ʈ�� �ִ´�.
		case TOKENR_MATERIAL:
		{
			// ���׸����� �ϳ� ���� ����Ʈ�� �ִ´�.
			Create_materialdata_to_list();

			m_materialdata->m_materialnumber = Parsing_NumberInt();
		}
		break;

		// ���� ������ �Ľ����ݽô�.
		case TOKENR_MATERIAL_NAME:
		{
			m_materialdata->m_material_name = Parsing_String();
		}
		break;
		case TOKENR_MATERIAL_CLASS:
		{
			m_materialdata->m_material_class = Parsing_String();
		}
		break;
		case TOKENR_MATERIAL_AMBIENT:
		{
			m_materialdata->m_material_ambient = Parsing_NumberVector3Mat();
		}
		break;
		case TOKENR_MATERIAL_DIFFUSE:
		{
			m_materialdata->m_material_diffuse = Parsing_NumberVector3Mat();
		}
		break;
		case TOKENR_MATERIAL_SPECULAR:
		{
			m_materialdata->m_material_specular = Parsing_NumberVector3Mat();
		}
		break;
		case TOKENR_MATERIAL_SHINE:
		{
			m_materialdata->m_material_shine = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_SHINESTRENGTH:
		{
			m_materialdata->m_material_shinestrength = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_TRANSPARENCY:
		{
			m_materialdata->m_material_transparency = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_WIRESIZE:
		{
			m_materialdata->m_material_wiresize = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_SHADING:
		{
			// ASE������ Blinn�� ���� ������ ���� �ִ�.
			m_materialdata->m_material_shading = Parsing_NumberInt();
		}
		break;
		case TOKENR_MATERIAL_XP_FALLOFF:
		{
			m_materialdata->m_material_xp_falloff = Parsing_NumberFloat();
		}
		break;

		// ��ǻ�� ���� �����ٸ�?
		case TOKENR_MAP_DIFFUSE:
		{
			// �� ���� ���� �߰����ش�.
			MaterialMap* _tempMap = new MaterialMap();

			m_materialdata->m_map_diffuse = _tempMap;
			m_parsingmode = eParsingmode::eDiffuseMap;
		}
		break;
		// ���̸� �߰�
		case TOKENR_MAP_NAME:
		{
			m_materialdata->m_map_diffuse->m_map_name = Parsing_String();
		}
		break;
		case TOKENR_MAP_CLASS:
		{
			m_materialdata->m_map_diffuse->m_map_class = Parsing_String();
		}
		break;
		case TOKENR_MAP_SUBNO:
		{
			m_materialdata->m_map_diffuse->m_subno = Parsing_NumberInt();
		}
		break;
		case TOKENR_MAP_AMOUNT:
		{
			m_materialdata->m_map_diffuse->m_map_amount = Parsing_NumberFloat();
		}
		break;
		// �ϴ��� ���� �ٽ����� ��Ʈ�ʱ�����..(�ؽ��� ��ġ �ľ�)
		case TOKENR_BITMAP:
		{
			if (m_parsingmode == eDiffuseMap)
			{
				m_materialdata->m_map_diffuse->m_bitmap = Parsing_String();
			}
		}
		break;
		// ��� �����ϸ� Ÿ�ϸ��̳� UV offset�� ó�����ָ� ����������.. �ϴ��� ��Ʈ�� ������ �� �� node�� �Ѿ�ڽ��ϴ�..
		// ���������� ��������
		case TOKENR_MAP_BUMP:
		{
			// BitMap �ߺ��� ���� ���� �Ľ̸�� ����
			m_parsingmode = eParsingmode::eNone;
		}
		break;

		//--------------------
		// GEOMOBJECT
		//--------------------

		case TOKENR_GROUP:
			//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
			break;

		case TOKENR_HELPEROBJECT:
			// �ϴ� �����ϰ�
			// ������Ʈ�� Ÿ�� ������. �̰Ϳ� ���� ���� �ٸ� �Ľ� ��� �ߵ�.
		{
			// Mesh�� ������ ������ ������ �־���ϱ� ������..
			Create_onemesh_to_list();

			m_parsingmode = eParsingmode::eHelperObject;
			// ���ۿ�����Ʈ�� Mesh�� �������� �ʱ� ������ ���õ� �κ��� 0���� ���ش�.
			m_TempGeomObject->m_mesh_numvertex = 0;
			m_TempGeomObject->m_mesh_numfaces = 0;
		}
		break;

		case TOKENR_GEOMOBJECT:
		{
			/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?

			Create_onemesh_to_list();

			// �Ľ̸�� ����.
			m_parsingmode = eParsingmode::eGeomobject;
		}
		break;

		case TOKENR_NODE_NAME:
			// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
			// ��忡 ���� �־�� �� ���� �ٸ���.
		{
			// �ϴ� ���� ������Ʈ �϶�����.. ���Ŀ� �ٸ� �ֵ��� �ٸ��� �־��ָ� ���� ������.
			switch (m_parsingmode)
			{
			case eParsingmode::eGeomobject:
			{
				// ���� �Ľ��ϴ� �޽ÿ� �̸��� �־��ش�.
				// ����Ʈ�� �� �ִ� ������ ���� ���� one_mesh�� �� �ִ� ������ ���� ���� ������
				// one_mesh�� ���ĵ� ����Ʈ�� �� �ִ� �޽��� ���� �ٲ� ��..
				m_TempGeomObject->m_nodename = Parsing_String();
			}
			break;
			case eParsingmode::eHelperObject:
			{
				// NODE_TM������ �ϳ� ������ ������, ��κ� �����ϴ� �����ϰ�..
				m_TempGeomObject->m_nodename = Parsing_String();
			}
			break;
			case eShape:
			{
				m_TempGeomObject->m_nodename = Parsing_String();
			}
			break;
			case eAnimation:
			{
				// �ִϸ��̼��̸�..
				m_animation->m_nodename = Parsing_String();
			}
			break;
			default:

				break;
			}
		}
		break;

		case TOKENR_HELPER_CLASS:
		{
			// ���� Ŭ���� Ȯ��..
			if (m_parsingmode == eHelperObject)
			{

			}
		}
		break;

		case TOKENR_NODE_PARENT:
			// �� ����� �θ� ����� ����.
			// �ϴ� �Է��� �ϰ�, ���߿� ��������.
			// ��带 �� ����������.
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eShape || m_parsingmode == eHelperObject)
			{
				m_TempGeomObject->m_nodeparent = Parsing_String();
				m_TempGeomObject->m_isparentexist = true;
			}
		}
		break;

		/// NODE_TM

		case TOKENR_NODE_TM:
			//m_parsingmode	=	eGeomobject;
			// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
			// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

			/// �Դٰ�,
			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
			// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
			// ��ͱ����� �������̴�....

			// �ϴ� ������ �Ѿ��.. Node�� �Ӹ� �����ڱ�...
				// ���� �Ľ� ��带 ��������� �ʱ� ������ + nodeName�� ���� ������ �׳� �Ѿ �ش�..
			break;

		case TOKENR_INHERIT_POS:
			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_inherit_pos = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_INHERIT_ROT:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_inherit_rot = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_INHERIT_SCL:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_inherit_scl = Parsing_NumberVector3();
			}
		}
		break;

		// ���� ����, �ϴ� ������ �а� ���߿� �ҷ� �� �� row1, 2�� ��ȯ ���ش�.
			// ������ col�� ��ȯ�Ѵ� ��������
		case TOKENR_TM_ROW0:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row0 = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROW1:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row1 = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROW2:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row2 = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROW3:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_row3 = Parsing_NumberVector3();
			}
		}
		break;

		// ������ �и��� ����
			// ���⵵ y, z�� �ٲ��ش�..
		case TOKENR_TM_POS:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_pos = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROTAXIS:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_rotaxis = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_ROTANGLE:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_rotangle = Parsing_NumberFloat();
			}
		}
		break;
		case TOKENR_TM_SCALE:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_scale = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_SCALEAXIS:
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_scaleaxis = Parsing_NumberVector3();
			}
		}
		break;
		case TOKENR_TM_SCALEAXISANG:
			// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
		{
			if (m_parsingmode == eGeomobject || m_parsingmode == eHelperObject || m_parsingmode == eShape)
			{
				m_TempGeomObject->m_tm_scaleaxisang = Parsing_NumberFloat();
			}
		}
		break;

		/// ANIMATION
		case TOKENR_TM_ANIMATION:
		{
			// �ִϸ��̼� ��ū�� ������ �ִϸ��̼��� �ϳ� �־��ش�.
			Create_animationdata_to_list();
			m_parsingmode = eAnimation;

			m_TempGeomObject->m_isAnimated = true;
			m_animation->m_ticksperFrame = m_scenedata.m_ticksperframe;
		}
		break;
		// �������� �־��..

		case TOKENR_CONTROL_POS_SAMPLE:
		{
			// �ٽ��� ���ÿ� ����ִ� ��..
			// �������� ���� �־��ش�.
			CAnimation_pos* _tempPos = new CAnimation_pos;
			_tempPos->m_time = Parsing_NumberInt();
			// y z ���� ����.
			_tempPos->m_pos = Parsing_NumberVector3();
			m_animation->m_position.push_back(_tempPos);
		}
		break;
		// �����̼��� �־��..

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			CAnimation_rot* _tempRot = new CAnimation_rot;
			_tempRot->m_time = Parsing_NumberInt();
			_tempRot->m_rot = Parsing_NumberVector3();
			_tempRot->m_angle = Parsing_NumberFloat();

			// ���ʹϾ��� ��� �־�����.. => �ִϸ��̼ǽÿ� angle�� ������ �����ؼ� ������ش�.
				// ���Ϸ����� ���ʹϾ� ��ȯ�� �ʼ���.. ��ȯ�� �־��ְ� �ش� ���� �ִϸ��̼ǿ��� ����Ѵ�..?

				// ���ʹϾ��� ������ְ� => �¼��� �ڵ忡�� ��Ʈ.
			tempQT = XMQuaternionRotationAxis(
				{ _tempRot->m_rot.x, _tempRot->m_rot.y, _tempRot->m_rot.z }
			, _tempRot->m_angle);

			// ���� ���� ������ �����Ѵ�..?
			if (m_animation->m_rotation.size() == 0)
			{
				_tempRot->m_rotQT_accumulation = tempQT;
			}
			else
			{
				// ���� ���ʹϾ��� �����ͼ� �����ش�.
				prevQT = m_animation->m_rotation.back()->m_rotQT_accumulation;
				_tempRot->m_rotQT_accumulation = prevQT * tempQT;
			}

			m_animation->m_rotation.push_back(_tempRot);
		}
		break; // �극��ũ��..
	// �������� ������ ���� ����.
		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			CAnimation_scl* _tempScl = new CAnimation_scl;
			_tempScl->m_time = Parsing_NumberInt();
			_tempScl->m_scale = Parsing_NumberVector3();
			_tempScl->m_scaleaxis = Parsing_NumberVector3();
			_tempScl->m_scaleaxisang = Parsing_NumberFloat();

			m_animation->m_scale.push_back(_tempScl);
		}
		break;

		/// MESH

		case TOKENR_MESH:
			//
			break;
		case TOKENR_TIMEVALUE:
		{
			m_TempGeomObject->m_timevalue = static_cast<float>(Parsing_NumberInt());
		}
		break;
		case TOKENR_MESH_NUMBONE:
			// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
		{
			// Skining..
			// GeomObj�� �������� �� �ִ�.
			m_TempGeomObject->m_is_skinningobject = true;
		}
		break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
			// ASE�� �ִ� ������ �־�����.
		case TOKENR_MESH_NUMVERTEX:
			m_TempGeomObject->m_mesh_numvertex = Parsing_NumberInt();
			break;
		case TOKENR_MESH_NUMFACES:
			m_TempGeomObject->m_mesh_numfaces = Parsing_NumberInt();
			break;

			/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			//
			// ���ؽ��� ������ ����־�� �ϴµ�
			// �̹� ���ͷ� ������ �� �����Ƿ� �׳� ������ �ȴ�.
			// ���ؽ� ����Ʈ�� �־��ֱ�. Position�� ���� �ε����� ���� ��ġ ���� ����.
		{
			// ����Ʈ���� � ������ �ִ°�..
				// ���������� �ٷ� ���ؽ� ��ū�� �־ ���⼭ � �۾��� ��������� �𸣰ڴ�..
		}
		break;
		case TOKENR_MESH_VERTEX:
			// ������ �Է�
				// ���� ���ؽ� ������ �� �ִ�..
		{
			// ���� ������� �־��ش�..?
			// ���ؽ��� �ϳ� �����ؼ� �־�����.
			Vertex* _temp = new Vertex();

			_temp->m_indices = Parsing_NumberInt();
			// y�� z�� �ٲ㼭 �־��ش�.
			/*
			temp->m_pos.x = Parsing_NumberFloat();
			temp->m_pos.z = Parsing_NumberFloat();
			temp->m_pos.y = Parsing_NumberFloat();
			 */

			 // �̹� �Լ��� �ִ�..
			_temp->m_pos = Parsing_NumberVector3();

			m_TempGeomObject->m_meshvertex.push_back(_temp);
		}
		break;

		/// Bone

		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_LIST:
		{
			m_parsingmode = eBone_List;
		}
		break;
		case TOKENR_BONE:
		{
			/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
			// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
			// Bone�� �ѹ��� �о� ����
			if (m_parsingmode = eBone_List)
			{
				Bone* _tempBone = new Bone;
				_tempBone->m_bone_number = Parsing_NumberInt();
				m_TempGeomObject->m_bone = _tempBone;
				m_TempGeomObject->m_vector_bone_list.push_back(_tempBone);
			}
		}
		break;
		//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
		case TOKENR_BONE_NAME:
		{
			m_TempGeomObject->m_bone->m_bone_name = Parsing_String();
		}
		break;
		case TOKENR_BONE_PROPERTY:
			// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
			break;
			// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

		case TOKENR_MESH_WVERTEXS:
			// ���� ���� ���� ����.
			break;

		case TOKENR_MESH_WEIGHT:
		{
			// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
				// ������ ���ؽ��� ���ؽ��� �ɰ��� ���� ���ؽ� ���� ������ ��ġ�Ѵ�
				// => ���⼭ �ɰ��� ���ؽ��鵵 ���� Weight�� ������ �־���Ѵ�.
			VertexWeight* _tempWVertex = new VertexWeight;
			m_TempGeomObject->m_wvertex = _tempWVertex;
			_tempWVertex->m_wvertex_number = Parsing_NumberInt();
			m_TempGeomObject->m_vector_wvertexs.push_back(_tempWVertex);
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			// ��ü �� �ܰ踦 ���°ž�...
			// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
			/// �� ��....
			Weight* _tempBoneWeight = new Weight;
			_tempBoneWeight->m_bone_number = Parsing_NumberInt();
			_tempBoneWeight->m_bone_weight = Parsing_NumberFloat();
			m_TempGeomObject->m_wvertex->m_temp_bone_blend_weight = _tempBoneWeight;

			// ���� �Ľ��ϰ� �ִ� ���ؽ��� ����Ʈ�� �־��ش�.
			m_TempGeomObject->m_wvertex->m_bone_blending_weight.push_back(_tempBoneWeight);
		}
		break;


		/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			//
			break;
		case TOKENR_MESH_FACE:
		{
			// Face�� ��ȣ�ε�...

			// A:�� �а�
			// B:
			// C:

			/// (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
			/// ......

			// ���Ϳ� �־��ش�.
				// ���� ���ؽ��� ���� ������� �־��ش�. ��� �̶� �ε��� ������ ABC�� �ƴ� �ݽð踦 ���������..
				// A: �� ���� ���� �ٲ����..

			Face* _temp = new Face();
			// temp�� ������ �־���� �ϴµ�..
			// �� ���� �� ������ �͵��� �������� �� �ֳ�..
				// ù��° �ε��� ��ȣ�� ����..
			Parsing_NumberInt();
			Parsing_String();
			// �� ����° ���� �����´�..
				// �� �ϴ� load Goem���� ������ �Ŷ� �ϰ� �׳� �־�����.
			_temp->m_vertexindex[0] = Parsing_NumberInt();
			Parsing_String();
			_temp->m_vertexindex[1] = Parsing_NumberInt();
			Parsing_String();
			_temp->m_vertexindex[2] = Parsing_NumberInt();

			// �׸��� ��ġ ����..

			// ����Ʈ�� �ֱ�
			m_TempGeomObject->m_meshface.push_back(_temp);
		}
		break;

		// Normal
		case TOKENR_MESH_FACENORMAL:
		{
			// ���� �ε����� ���̽��� ���̽� �븻�� �־��ش�.
			int _tempIdx = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_normal = Parsing_NumberVector3();
		}
		break;

		// ���ؽ� �븻.. => �ش� ���ؽ� �ε����� �������� ������ ���ο� ���ؽ��� ���� �ִ´�..
		case TOKENR_MESH_VERTEXNORMAL:
		{
			// ù��° ���ؽ� �ε����� �Ѱ��ֱ� ���� �Ľ����ش�.
			Parsing_NumberInt();

			int _tempFaceIdx = _tempFace / 3;
			int _normalVertexIdx = _tempFace % 3;

			m_TempGeomObject->m_meshface[_tempFaceIdx]->m_normalvertex[_normalVertexIdx] = Parsing_NumberVector3();

			_tempFace++;
		}
		break;

		case TOKENR_MESH_NUMTVERTEX:
		{
			// TVertex�� ������ �߰��Ѵ�.
			m_TempGeomObject->m_mesh_numtvertex = Parsing_NumberInt();
		}
		break;

		/// MESH_TVERTLIST

		case TOKENR_MESH_TVERTLIST:
			//
			break;
		case TOKENR_MESH_TVERT:
		{
			// ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
			Parsing_NumberInt();
			// ���ο� TVertex�� ���� ���Ϳ� �ִ´�
			// COneTVertex�� ���� ���� ���Ϳ� �־��ش�.
			COneTVertex* _tempTVertex = new COneTVertex();
			_tempTVertex->m_u = Parsing_NumberFloat();
			_tempTVertex->m_v = Parsing_NumberFloat();
			_tempTVertex->m_w = Parsing_NumberFloat();

			m_TempGeomObject->m_mesh_tvertex.push_back(_tempTVertex);
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
		{
			m_TempGeomObject->m_mesh_tvfaces = Parsing_NumberInt();
		}
		break;
		case TOKENR_MESH_TFACE:
		{
			// FACE�� Ư�� �ε����� �ش��ϴ� TVertexindex ���� �����Ѵ�.
				// FACE 0 0, 2, 3 == TFACE 0 9, 11, 10
				// 0, 2, 3�� �׷��ٶ�, 9 11 10�� �ؽ��� ��ǥ�� �����ͼ� �׷��ش�.
			// ���� �ε����� ���̽��� ���̽� �븻�� �־��ش�.
			int _tempIdx = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_TFace[0] = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_TFace[1] = Parsing_NumberInt();
			m_TempGeomObject->m_meshface[_tempIdx]->m_TFace[2] = Parsing_NumberInt();
		}
		break;

		// SHAPE OBJ
		case TOKENR_SHAPEOBJECT:
		{
			// Mesh�� ������ ������ ������ �־���ϱ� ������..
			Create_onemesh_to_list();

			m_parsingmode = eParsingmode::eShape;
			// shape������Ʈ�� Geom������ �������� �ʱ� ������(?) ���õ� �κ��� 0���� ���ش�.
			m_TempGeomObject->m_mesh_numvertex = 0;
			m_TempGeomObject->m_mesh_numfaces = 0;
		}
		break;

		case TOKEND_END:
			// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

			//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
			TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
			return;

			break;

			/// ���� �ƹ��͵� �ش����� ������
		default:
			// �ƹ��͵� ���� �ʴ´�.
			break;

		}	// switch()


		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3���� Float�� ���� �ϳ���
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

Vector3 CASEParser::Parsing_NumberVector3Mat()
{
	LONG				token;
	Vector3			tempVector3;

	// �⺻������ ���� �Լ��� ������ y, z���� �ٲ��� �ʴ´�.
		// ��� rgb ���̱� ����
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);

	return			tempVector3;
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	GeomObject* temp = new GeomObject;
	m_TempGeomObject = temp;
	m_TempGeomObject->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_GeomObjList.push_back(m_TempGeomObject);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// ���� �ϳ���..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_TempGeomObject->m_meshvertex.push_back(temp);
}

















